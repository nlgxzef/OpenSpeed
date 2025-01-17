// clang-format off
//
//    MemoryEditor: A header-only cross-platform library to edit runtime memory. (C++11)
//    Copyright (C) 2022 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#include <array>
#include <cstdint>  // integer types
#include <cstring>  // memcpy
#include <initializer_list>
#include <memory>  // unique_ptr
#include <mutex>   // mutex, scoped_lock
#include <unordered_map>
#include <type_traits>  // enable_if, is_arithmetic
#include <tuple>

#if defined(__linux__) || defined(_LINUX)
#include <sys/mman.h>  // mprotect()
#elif defined(_WIN32)
#include <libloaderapi.h>  // GetModuleHandle()
#include <memoryapi.h>     // VirtualProtect()
#else
#error This operating system is not supported.
#endif

namespace MemoryEditor {
  enum class MakeType : std::uint8_t {
    // 0xE8
    Call,
    // 0xE9
    Jump,
    // 0x90
    NOP,
    // 0xC3
    Return,
    // 0xCC
    DebuggerTrap
  };

  class Editor {
   public:
    class DetourInfo {
      bool mHasDetoured;

     protected:
      std::array<std::uint8_t, sizeof(std::uint32_t) + 1> mOrigBytes;
      std::uintptr_t                                      mAddrFrom;
      std::uintptr_t                                      mAddrDetour;

     public:
      std::uintptr_t GetAddrFrom() const { return mAddrFrom; }
      std::uintptr_t GetAddrDetour() const { return mAddrDetour; }
      bool           GetHasDetoured() const { return mHasDetoured; }

      void Detour() {
        if (mHasDetoured) return;

        Editor::Get().Make(MakeType::Jump, mAddrFrom, mAddrDetour);
        mHasDetoured = true;
      }
      void Undetour() {
        if (!mHasDetoured) return;

        Editor::Get().UnlockMemory(mAddrFrom, sizeof(std::uint32_t) + 1);
        std::memcpy(reinterpret_cast<void*>(mAddrFrom), mOrigBytes.data(), sizeof(std::uint32_t) + 1);
        Editor::Get().LockMemory(mAddrFrom);
        mHasDetoured = false;
      }

      // Detours immediately
      explicit DetourInfo(std::uintptr_t addrFrom, std::uintptr_t addrDetour) :
          mHasDetoured(false), mAddrFrom(addrFrom), mAddrDetour(addrDetour) {
        Editor::Get().UnlockMemory(mAddrFrom, sizeof(std::uint32_t) + 1);
        std::memcpy(mOrigBytes.data(), reinterpret_cast<void*>(mAddrFrom), sizeof(std::uint32_t) + 1);
        Editor::Get().LockMemory(mAddrFrom);

        Detour();
      }
    };

   protected:
    struct MemoryAccessInfo {
#if defined(_WIN32)
      DWORD oldMemoryAccess = NULL;
#endif
      std::uintmax_t size = NULL;

      constexpr MemoryAccessInfo() = default;
      constexpr MemoryAccessInfo(std::uintmax_t infoSize) : size(infoSize) {}
    };

    std::uintptr_t                                               mBase;
    mutable std::mutex                                           mMutex;
    mutable std::unordered_map<std::uintptr_t, MemoryAccessInfo> mAccessInfos;

    inline std::uint32_t CalcDistance(std::uintptr_t from, std::uintptr_t to) const {
      return to - from - sizeof(std::uint32_t) - 1;
    }

    bool ValidateMemoryIsInitializedInternal(std::uintptr_t address) const {
#if defined(_WIN32)
      // Check if page is accessible
      {
        MEMORY_BASIC_INFORMATION mbi;
        ZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));
        if (!::VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi))) return false;
        // Ensure r/w/x
        if (!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
                             PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
          return false;
        // Ensure no guard or no access
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) return false;
      }
#endif

      // VC++ Magic Numbers //

      // Guard bytes after allocated heap memory
      if (*reinterpret_cast<std::uint32_t*>(address) == 0xABABABAB) return false;
      // Uninitialized stack memory
      if (*reinterpret_cast<std::uint32_t*>(address) == 0xCCCCCCCC) return false;
      // Uninitialized heap memory
      if (*reinterpret_cast<std::uint32_t*>(address) == 0xCDCDCDCD) return false;
      // Guard bytes before & after allocated heap memory
      if (*reinterpret_cast<std::uint32_t*>(address) == 0xFDFDFDFD) return false;
      // Freed memory
      if (*reinterpret_cast<std::uint32_t*>(address) == 0xFEEEFEEE) return false;

      return true;
    }

    explicit Editor() {
#if defined(__linux__) || defined(_LINUX)
#error Base address cannot be dynamically acquired without hacks on linux systems. Call 'Get(baseAddress)' instead.
#elif defined(_WIN32)
      mBase = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(NULL));
#endif
    }
    explicit Editor(std::uintptr_t base) : mBase(base) {}

   public:
    inline std::uintptr_t AbsRVA(std::uintptr_t rva) const { return mBase + rva; }

    void LockMemory(std::uintptr_t address) const {
      std::scoped_lock<std::mutex> _lock(mMutex);
#if defined(_WIN32)
      // Old memory access page is stored only in Windows.

      MemoryAccessInfo& info = mAccessInfos[address];
      VirtualProtect(reinterpret_cast<LPVOID>(address), static_cast<SIZE_T>(info.size), info.oldMemoryAccess,
                     &info.oldMemoryAccess);
#endif
      mAccessInfos.erase(address);
    }
    void UnlockMemory(std::uintptr_t address, std::size_t size) const {
      std::scoped_lock<std::mutex> _lock(mMutex);
      MemoryAccessInfo             _info(size);

#if defined(__linux__) || defined(_LINUX)
      mprotect(reinterpret_cast<void*>(address), size, PROT_READ | PROT_WRITE | PROT_EXEC);
#elif defined(_WIN32)
      VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_EXECUTE_READWRITE, &_info.oldMemoryAccess);
#endif
      mAccessInfos[address] = _info;
    }

    std::unique_ptr<DetourInfo> Detour(std::uintptr_t from, std::uintptr_t to) const {
      return std::make_unique<DetourInfo>(from, to);
    }
    void Make(MakeType type, std::uintptr_t from, std::uintptr_t to) const {
      std::uint8_t* _arr = reinterpret_cast<std::uint8_t*>(from);

      std::uint8_t _b = 0x00;
      switch (type) {
        case MakeType::Call:
          UnlockMemory(from, sizeof(std::uint32_t) + 1);
          _arr[0]                                     = 0xE8;
          *reinterpret_cast<std::uint32_t*>(&_arr[1]) = CalcDistance(from, to);
          break;
        case MakeType::Jump:
          UnlockMemory(from, sizeof(std::uint32_t) + 1);
          _arr[0]                                     = 0xE9;
          *reinterpret_cast<std::uint32_t*>(&_arr[1]) = CalcDistance(from, to);
          break;
        case MakeType::NOP:
          _b = 0x90;
          break;
        case MakeType::DebuggerTrap:
          _b = 0xCC;
          break;
        case MakeType::Return:
          _b = 0xC3;
          break;
      }

      if (_b != 0x00 && (to - from) > 0) {
        const std::uintptr_t _dist = to - from;
        UnlockMemory(from, _dist);
        for (std::uintptr_t i = from; i < _dist; i++) *reinterpret_cast<std::uint8_t*>(i) = _b;
      }
      LockMemory(from);
    }

    bool ValidateMemoryIsInitialized(std::uintptr_t address) const {
      UnlockMemory(address, sizeof(std::uint32_t));
      bool _ret = ValidateMemoryIsInitializedInternal(address);
      LockMemory(address);

      return _ret;
    }
    template <typename PtrType>
    bool ValidateMemoryIsInitialized(PtrType* ptr) const {
      if (!ptr) return false;
      return ValidateMemoryIsInitialized(reinterpret_cast<std::uintptr_t>(ptr));
    }

    template <typename PointedType>
    PointedType* ReadPointer(std::uintptr_t base, std::initializer_list<std::uintptr_t> offsets) const {
      PointedType* _ret = nullptr;
      UnlockMemory(base, sizeof(std::uintptr_t));
      if (!base || !ValidateMemoryIsInitializedInternal(base) || !*reinterpret_cast<std::uintptr_t*>(base)) {
        LockMemory(base);
        return _ret;
      }

      std::uintptr_t _last = *reinterpret_cast<std::uintptr_t*>(base);
      LockMemory(base);

      for (const auto& off : offsets) {
        LockMemory(_last);
        std::uintptr_t  _offBase = _last + off;
        std::uintptr_t* _p       = reinterpret_cast<std::uintptr_t*>(_offBase);
        UnlockMemory(_offBase, sizeof(std::uintptr_t));
        if (!_p || !ValidateMemoryIsInitializedInternal(*_p)) {
          LockMemory(_offBase);
          _ret = nullptr;
          break;
        }
        _last = *_p;
        _ret  = reinterpret_cast<PointedType*>(_last);
      }
      return _ret;
    }

    static inline const Editor& Get() {
      static Editor memory;
      return memory;
    }
    static inline const Editor& Get(std::uintptr_t base) {
      static Editor memory(base);
      return memory;
    }
  };

  static inline const Editor& Get() { return Editor::Get(); }
  static inline const Editor& Get(std::uintptr_t base) { return Editor::Get(base); }
}  // namespace MemoryEditor
