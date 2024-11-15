#pragma once
#include "pch/framework.h"
#include "common/joaat.h"
#define rage_new new
#include "rgsc/public_interface/rgsc_common.h"
#include "rgsc/public_interface/presence_interface.h"
#include "rgsc/public_interface/rgsc_interface.h"
#include "rgsc/public_interface/titleid_interface.h"
#include "rgsc/public_interface/delegate_interface.h"
#include "rgsc/public_interface/configuration_interface.h"
#include "rgsc/public_interface/profiles_interface.h"
#include "rgsc/public_interface/pad_interface.h"
#include "rgsc/public_interface/players_interface.h"
#include "rgsc/public_interface/achievements_interface.h"
#include "rgsc/public_interface/commerce_interface.h"
#include "rgsc/public_interface/activation_interface.h"
#include "rgsc/public_interface/patching_interface.h"
#include "rgsc/public_interface/file_system_interface.h"
#include "rgsc/public_interface/rgsc_ui_interface.h"
#include "rgsc/public_interface/telemetry_interface.h"
#include "rgsc/public_interface/network_interface.h"
#include "rgsc/public_interface/cloudsave_interface.h"
#include "rgsc/public_interface/gamerpics_interface.h"
#include "data/bitbuffer.h"
using namespace rage;

enum LoadingScreenMovieContext : std::uint32_t
{
	LOADINGSCREEN_SCALEFORM_CTX_BLANK,				// Blank Screen ( default value in the scaleform movie)
	LOADINGSCREEN_SCALEFORM_CTX_LEGAL,				// Legal Screen ( With the spinner icons and descriptions )
	LOADINGSCREEN_SCALEFORM_CTX_INSTALL,			// Installer Screen ( Uses a progress bar )
	LOADINGSCREEN_SCALEFORM_CTX_GAMELOAD,			// Game loading screen ( Uses a progress bar )
	LOADINGSCREEN_SCALEFORM_CTX_GAMELOAD_NEWS,		// Game loading (with network connectivity to retrieve rockstar news stories)
	LOADINGSCREEN_SCALEFORM_CTX_LANDING				// PC & Gen9 Landing Context (with network connectivity to retrieve rockstar news stories, but does not cycle backgrounds automatically)
};
enum LoadingScreenContext : std::uint32_t
{
	LOADINGSCREEN_CONTEXT_NONE,
	LOADINGSCREEN_CONTEXT_INTRO_MOVIE,				// renders the bink intro movie
	LOADINGSCREEN_CONTEXT_LEGALSPLASH,				// Legal non-scaleform splash screen. ( Startup context, can be passed in ::Init() )
	LOADINGSCREEN_CONTEXT_LEGALMAIN,				// Legal scaleform screen ( Startup context, can be passed in ::Init() )
	LOADINGSCREEN_CONTEXT_SWAP_UNUSED,				// Gen7 Legacy - Waiting for the disc swap. Exists just to keep hardcoded script values in sync with this enum
	LOADINGSCREEN_CONTEXT_LANDING,				    // PC & Gen9 Landing
	LOADINGSCREEN_CONTEXT_LOADGAME,					// Game loading scaleform screen ( Selected from within CLoadingScreens only)
	LOADINGSCREEN_CONTEXT_INSTALL,					// Game installing scaleform screen ( Selected from within CLoadingScreens only)
	LOADINGSCREEN_CONTEXT_LOADLEVEL,				// Level loading scaleform screen ( Startup context, can be passed in ::Init() )
	LOADINGSCREEN_CONTEXT_MAPCHANGE,				// Load screen when loading from SP to MP and vice versa through CExtraContentManager
	LOADINGSCREEN_CONTEXT_LAST_FRAME				// Use the last rendered frame as a background with a spinner as a loading screen
};
enum PM_COLUMNS
{
	PM_COLUMN_LEFT,
	PM_COLUMN_MIDDLE,
	PM_COLUMN_RIGHT,
	PM_COLUMN_EXTRA3,    // some pages have extra columns
	PM_COLUMN_EXTRA4,    // some pages have extra columns
	PM_COLUMN_LEFT_MIDDLE,
	PM_COLUMN_MIDDLE_RIGHT,
	PM_COLUMN_MAX
};
// Match with PAUSE_MENU_SP_CONTENT.as
enum PM_WARNING_IMG_ALIGNMENT
{
	IMG_ALIGN_NONE,
	IMG_ALIGN_TOP,
	IMG_ALIGN_RIGHT
};
enum eNetworkAccessCode
{
	Access_BankDefault = -2,
	Access_Invalid = -1,
	Access_Granted, 
	Access_Deprecated_TunableNotFound,
	Access_Deprecated_TunableFalse,
	Access_Denied_MultiplayerLocked,
	Access_Denied_InvalidProfileSettings,
	Access_Denied_PrologueIncomplete,
	Access_Denied_NotSignedIn,			/* Not signed into the Xbox profile */
	Access_Denied_NotSignedOnline,		/* Not signed into PSN or Xbox Live */
	Access_Denied_NoOnlinePrivilege,
	Access_Denied_NoRosCredentials,
	Access_Denied_NoRosPrivilege,
	Access_Denied_MultiplayerDisabled,
	Access_Denied_NoTunables,
	Access_Denied_NoBackgroundScript,

	// add new reasons for code after this
	// script enumerate anything above and would need an update 

	Access_Denied_NoNetworkAccess,		/* Ethernet or wifi is disconnected; only use by the landing page */
	Access_Denied_RosBanned,
	Access_Denied_RosSuspended,
};
enum eNetworkAccessArea
{
	AccessArea_Invalid,
	AccessArea_First = 0,
	AccessArea_Landing = AccessArea_First,
	AccessArea_Multiplayer,
	AccessArea_MultiplayerEnter,
	AccessArea_Num,
};
enum eBailReason
{
	BAIL_INVALID = -1,
	BAIL_FROM_SCRIPT,
	BAIL_FROM_RAG,
	BAIL_PROFILE_CHANGE,
	BAIL_NEW_CONTENT_INSTALLED,
	BAIL_SESSION_FIND_FAILED,
	BAIL_SESSION_HOST_FAILED,
	BAIL_SESSION_JOIN_FAILED,
	BAIL_SESSION_START_FAILED,
	BAIL_SESSION_ATTR_FAILED,
	BAIL_SESSION_MIGRATE_FAILED,
	BAIL_PARTY_HOST_FAILED,
	BAIL_PARTY_JOIN_FAILED,
	BAIL_JOINING_STATE_TIMED_OUT,
	BAIL_NETWORK_ERROR,
	BAIL_TRANSITION_LAUNCH_FAILED,
	BAIL_END_TIMED_OUT,
	BAIL_MATCHMAKING_TIMED_OUT,
	BAIL_CLOUD_FAILED,
	BAIL_COMPAT_PACK_CONFIG_INCORRECT,
	BAIL_CONSOLE_BAN,
	BAIL_MATCHMAKING_FAILED,
	BAIL_ONLINE_PRIVILEGE_REVOKED,
	BAIL_SYSTEM_SUSPENDED,
	BAIL_EXIT_GAME,
	BAIL_TOKEN_REFRESH_FAILED,
	BAIL_CATALOG_REFRESH_FAILED,
	BAIL_SESSION_REFRESH_FAILED,
	BAIL_SESSION_RESTART_FAILED,
	BAIL_GAME_SERVER_MAINTENANCE,
	BAIL_GAME_SERVER_FORCE_BAIL,
	BAIL_GAME_SERVER_HEART_BAIL,
	BAIL_GAME_SERVER_GAME_VERSION,
	BAIL_CATALOGVERSION_REFRESH_FAILED,
	BAIL_CATALOG_BUFFER_TOO_SMALL,
	BAIL_INVALIDATED_ROS_TICKET,
	BAIL_NUM,
};
// Quick and diry WSSL ctx impl
typedef struct DerBuffer
{
	u8* buffer;
	void* heap;
	u32 length;
	int type, dynType;
} DerBuffer;
struct WOLFSSL_CTX
{
	void* method;
	CRITICAL_SECTION countMtx;
	int refCount, err;
	struct buffer
	{
		u8* buffer;
		u32 length;
	} serverDH_P, serverDH_G;
	DerBuffer* certificate;
	DerBuffer* certChain;
	DerBuffer* privateKey;
	u8 privateKeyType : 6;
	u8 privateKeyId : 1;
	u8 privateKeyLabel : 1;
	int privateKeySz, privateKeyDevId;
};
typedef WOLFSSL_CTX SSL_CTX;
// R* stubs, yes, these exist and they're engine accurate
struct tagRGNDATA;
namespace rgsc
{
	class Rgsc
	{
	public:
		IRgscDelegate* m_Delegate;
		IRgscDelegateV1* m_DelegateV1;
		IRgscDelegateV2* m_DelegateV2;
		IRgscDelegateV3* m_DelegateV3;
		IFileDelegate* m_FileDelegate;
		TitleId m_TitleId;
		bool* IsUiEnabled()
		{
			return reinterpret_cast<bool*>(reinterpret_cast<u64>(this) + 0xDB913);
		}
	};
	//0xD6F0
}
//diagXXXXXX
namespace rage
{
	enum diagSeverity
	{
		DIAG_SEVERITY_FATAL,
		DIAG_SEVERITY_ASSERT,
		DIAG_SEVERITY_ERROR,
		DIAG_SEVERITY_WARNING,
		DIAG_SEVERITY_DISPLAY,
		DIAG_SEVERITY_DEBUG1,
		DIAG_SEVERITY_DEBUG2,
		DIAG_SEVERITY_DEBUG3,
		DIAG_SEVERITY_COUNT
	};
	enum diagChannelPosix
	{
		CHANNEL_POSIX_UNSPECIFIED = 0,	// use default (off) or inherit parent
		CHANNEL_POSIX_OFF,				// expressly off
		CHANNEL_POSIX_ON,				// expressly on
	};
	class diagChannel
	{
	public:
		diagChannel(diagChannel* parent, const char* tag, const char* paramTag, diagSeverity fileLevel = DIAG_SEVERITY_DEBUG1, diagSeverity ttyLevel = DIAG_SEVERITY_DISPLAY, diagSeverity popupLevel = DIAG_SEVERITY_ASSERT, diagChannelPosix posixPrefix = CHANNEL_POSIX_UNSPECIFIED);
		diagChannel() = default;
		u8 FileLevel, TtyLevel, PopupLevel, MaxLevel;
		u8 ParentCount; // Number of parents (could easily be u8)
		u8 OriginalFileLevel; //stores the original file severity level when setting an override
		u8 OriginalTtyLevel; //stores the original tty severity level when setting an override
		diagChannelPosix PosixPrefix;
	private:
		diagChannel* Parent;
		diagChannel* Next;
	};
}
//datXXXXXXX
namespace rage {
	const u64 g_rscVirtualLeafSize_WIN32 = 8192;
	const u64 g_rscPhysicalLeafSize_WIN32 = 8192;
	const u64 g_rscVirtualLeafSize = g_rscVirtualLeafSize_WIN32;
	const u64 g_rscPhysicalLeafSize = g_rscPhysicalLeafSize_WIN32;
	struct datResourceInfo
	{
		struct Sizes
		{
			u32 LeafShift : 4;
			u32 Head16Count : 1, Head8Count : 2, Head4Count : 4, Head2Count : 6;
			u32 BaseCount : 7;
			u32 HasTail2 : 1, HasTail4 : 1, HasTail8 : 1, HasTail16 : 1;
			u32 Version : 4;
			u32 GetCount() const
			{
				return Head16Count + Head8Count + Head4Count + Head2Count + BaseCount + HasTail2 + HasTail4 + HasTail8 + HasTail16;
			}
			u32 GetSize(u32 leafSize) const
			{
				leafSize <<= LeafShift;
				return (((Head16Count << 4) + (Head8Count << 3) + (Head4Count << 2) + (Head2Count << 1) + BaseCount) * leafSize)
					+ (HasTail2 ? (leafSize >> 1) : 0) + (HasTail4 ? (leafSize >> 2) : 0) + (HasTail8 ? (leafSize >> 3) : 0) + (HasTail16 ? (leafSize >> 4) : 0);
			}
		};
		u64 GetVirtualSize() const
		{
			return Virtual.GetSize(g_rscVirtualLeafSize);
		}
		u64 GetPhysicalSize() const
		{
			return Physical.GetSize(g_rscPhysicalLeafSize);
		}
		u64 GetVirtualChunkSize() const
		{
			return g_rscVirtualLeafSize << Virtual.LeafShift;
		}
		u64 GetPhysicalChunkSize() const
		{
			return g_rscPhysicalLeafSize << Physical.LeafShift;
		}
		u64 GetVirtualChunkCount() const
		{
			return Virtual.GetCount();
		}
		u64 GetPhysicalChunkCount() const
		{
			return Physical.GetCount();
		}
		int GetVersion() const
		{
			return (Virtual.Version << 4) | (Physical.Version);
		}
		Sizes Virtual;
		Sizes Physical;
	};
	struct datResourceChunk
	{
		static constexpr u32 MAX_CHUNKS = 128;
		void* SrcAddr;
		void* DestAddr;
		u64 Size;
	};
	struct datResourceMap
	{
		u8 VirtualCount, PhysicalCount;
		u8 RootVirtualChunk, DisableMerge;
		u8 HeaderType;
		void* VirtualBase;
		datResourceChunk Chunks[datResourceChunk::MAX_CHUNKS];
		int LastSrc, LastDest;
	};
	template <class _T>
	struct datRef
	{
		datRef(_T* p) : ptr(p)
		{}
		_T& operator*() const
		{
			return *ptr;
		}
		_T* operator->() const
		{
			return ptr;
		}
		operator _T* () const
		{
			return ptr;
		}
		_T*& operator=(_T* that)
		{
			return ptr = that;
		}
		_T* ptr;
	};
	template <class _T>
	struct datOwner
	{
		datOwner(_T* p) : ptr(p)
		{}
		_T& operator*() const
		{
			return *ptr;
		}
		_T* operator->() const
		{
			return ptr;
		}
		operator _T* () const
		{
			return ptr;
		}
		_T*& operator=(_T* that)
		{
			return ptr = that;
		}
		_T* ptr;
	};
	class datBase
	{
	public:
		virtual ~datBase() = default;
	};
	template <u32 _Count, typename _T = u8> struct datPadding
	{
	public:
		bool operator==(int n)
		{
			return (n == 0);
		}
	private:
		_T m[_Count];
	};
	typedef void *fiHandle;
	class datGrowBuffer
	{
	public:
		void* GetBuffer()
		{
			return m_Len ? m_Buf : NULL;
		}
		u8* m_Buf;
		u32 m_Capacity;
		u32 m_Len;
		u32 m_Flags;
		void* m_Allocator;
		u32 m_growIncr;  //Multiples of size to grow the buffer in.
		fiHandle m_FiHandle;
	};
	template<unsigned VAL>
	struct datBitsNeeded
	{
		static const u32 COUNT = VAL ? (datBitsNeeded<(VAL >> 1)>::COUNT + 1) : 0;
	};
	template<>
	struct datBitsNeeded<0>
	{
		static const u32 COUNT = 0;
	};
}
//atXXXXXXX
namespace rage {
	template <u64 X>
	struct CompileTimeLog2Floor
	{
		enum
		{
			value = 1 + CompileTimeLog2Floor<(X >> 1)>::value
		};
	};
	template<>
	struct CompileTimeLog2Floor<1>
	{
		enum
		{
			value = 0
		};
	};
	class atHashValue
	{
	public:
		void SetHash(const u32 hash)
		{
			m_hash = hash;
		}
		u32 GetHash() const
		{
			return m_hash;
		}
		void Clear()
		{
			m_hash = 0;
		}
		bool IsNull() const
		{
			return m_hash == 0;
		}
		bool IsNotNull() const
		{
			return m_hash != 0;
		}
	protected:
		u32 m_hash;
	};
	template<int, typename> class atFixedBitSetIterator;
	template <int Size, typename Type = u32>
	class atFixedBitSetBase
	{
		template<int X, typename Y> friend class atFixedBitSetIterator;
	protected:
		static constexpr int BlockSize = sizeof(Type);
		static constexpr int BitsPerBlock = BlockSize * 8;
		static constexpr int BlockBitShift = CompileTimeLog2Floor<BitsPerBlock>::value;
		static constexpr int NumBlocks = (((Size)+(BitsPerBlock - 1)) >> BlockBitShift);
	public:
		enum
		{
			NUM_BITS = Size,
			NUM_BLOCKS = NumBlocks,
			BLOCK_SIZE = BlockSize
		};
		typedef atFixedBitSetBase<Size, Type> myType;
	protected:
		Type m_Bits[NumBlocks];
		__forceinline void ClearUndefinedBits()
		{
			const int numOnes = ((Size - 1) % BitsPerBlock) + 1; // numOnes is 1..32
			const Type ones = (Type)~(Type)0x0u; // ones = 0xFFFFFFFF
			const Type mask = ones >> (BitsPerBlock - numOnes); // Shift by (32 - (1..32)) = (31..0)
			m_Bits[NumBlocks - 1] &= mask;
		}
		__forceinline Type& Block(u32 i)
		{
			return m_Bits[i >> BlockBitShift];
		}
		__forceinline const Type& Block(u32 i) const
		{
			return m_Bits[i >> BlockBitShift];
		}
		__forceinline Type BitInBlock(u32 i) const
		{
			return (Type)1 << (i & (BitsPerBlock - 1));
		}
	};
	template <int Size, typename Type = u32>
	class atFixedBitSet : public atFixedBitSetBase<Size, Type>
	{
	public:
		typedef atFixedBitSetBase<Size, Type> myBaseType;
		typedef atFixedBitSet<Size, Type> myType;
	};
	#define bsWordSize (((Size)+31)>>5)
	typedef atFixedBitSet<8, u8> atFixedBitSet8;
	typedef atFixedBitSet<16, u16> atFixedBitSet16;
	typedef atFixedBitSet<32, u32> atFixedBitSet32;
	class ConstStringBase
	{
	public:
		operator const char* () const
		{
			return c_str();
		}
		const char* c_str() const
		{
			return m_String;
		}
		const char* m_String;
	};
	class ConstString : public ConstStringBase
	{
	public:

	};
	class atStringBuilder
	{
	public:
		const char* ToString()
		{
			char* str = (char*)m_DBuf.GetBuffer();
			return str ? str : "";
		}
		char* ToData()
		{
			return (char*)m_DBuf.GetBuffer();
		}
		datGrowBuffer m_DBuf;
	};
	class atHashString
	{
	public:
		atHashString(u32 hash) : m_hash(hash)
		{}
		atHashString(const char* hashKey) : m_hash(atStringHash(hashKey))
		{}
		atHashString()
		{}
		u32 m_hash;
	};
	typedef atHashString atHashWithStringBank;
	typedef atHashString atFinalHashString;
	class atString
	{
	public:
		const char* c_str() const
		{
			return m_Length ? m_Data : "";
		}

		char* m_Data;
		u16 m_Length, m_Allocated;
	};
	template <typename DataT, typename CountT = u16>
	class atArray
	{
	public:
		DataT* begin()
		{
			return m_Elements;
		}
		DataT* end()
		{
			return m_Elements + m_Count;
		}
		const DataT* begin() const
		{
			return m_Elements;
		}
		const DataT* end() const
		{
			return m_Elements + m_Count;
		}
		DataT* data()
		{
			return m_Elements;
		}
		const DataT* data() const
		{
			return m_Elements;
		}
		CountT size() const
		{
			return m_Count;
		}
		CountT capacity() const
		{
			return m_Capacity;
		}
		DataT& operator[](CountT index)
		{
			return m_Elements[index];
		}
		const DataT& operator[](CountT index) const
		{
			return m_Elements[index];
		}
		DataT& push_back(int beforeIndex)
		{
			for (int i = m_Count; i > beforeIndex; i--)
				m_Elements[i] = m_Elements[i - 1];
			++m_Count;
			return m_Elements[beforeIndex]; //lint !e797
		}
		int binary_search(const CountT& t) const
		{
			int low = 0, high = m_Count - 1;
			while (low <= high)
			{
				int mid = (low + high) >> 1;
				if (t == m_Elements[mid])
					return mid;
				else if (t < m_Elements[mid])
					high = mid - 1;
				else
					low = mid + 1;
			}
			return -1;
		}
	private:
		DataT* m_Elements;
		CountT m_Count, m_Capacity;
	};
	template <typename DataT, s32 Capacity>
	class atFixedArray
	{
	public:
		DataT* begin()
		{
			return m_Elements;
		}
		DataT* end()
		{
			return m_Elements + m_Count;
		}
		const DataT* begin() const
		{
			return m_Elements;
		}
		const DataT* end() const
		{
			return m_Elements + m_Count;
		}
		DataT* data()
		{
			return m_Elements;
		}
		const DataT* data() const
		{
			return m_Elements;
		}
		s32 size() const
		{
			return m_Count;
		}
		s32 capacity() const
		{
			return Capacity;
		}
		DataT& operator[](s32 index)
		{
			return m_Elements[index];
		}
		const DataT& operator[](s32 index) const
		{
			return m_Elements[index];
		}
	private:
		DataT* m_Elements;
		s32 m_Count = Capacity;
	};
	template <typename DataT, s32 MaxCount>
	class atRangeArray
	{
	public:
		DataT* begin()
		{
			return m_Elements;
		}
		DataT* end()
		{
			return m_Elements + MaxCount;
		}
		const DataT* begin() const
		{
			return m_Elements;
		}
		const DataT* end() const
		{
			return m_Elements + MaxCount;
		}
		DataT* data()
		{
			return m_Elements;
		}
		const DataT* data() const
		{
			return m_Elements;
		}
		s32 size() const
		{
			return MaxCount;
		}
		s32 capacity() const
		{
			return size();
		}
		DataT& operator[](s32 index)
		{
			return m_Elements[index];
		}
		const DataT& operator[](s32 index) const
		{
			return m_Elements[index];
		}
	private:
		DataT m_Elements[MaxCount];
	};
	template <typename _T, typename _KeyType>
	class atBinaryMap
	{
	public:
		class Iterator
		{
		public:
			Iterator() : Map(NULL), CurIdx(0)
			{}
			explicit Iterator(atBinaryMap<_T, _KeyType>& themap) :Map(&themap), CurIdx(0)
			{}
			Iterator& operator++()
			{
				if (++CurIdx >= Map->Data.GetCount())
					*this = Map->End();
				return *this;
			}
			Iterator operator++(const int)
			{
				Iterator result(*this);
				++(*this);
				return result;
			}
			_T& operator*() const
			{
				return Map->Data[CurIdx].data;
			}
			_T* operator->() const
			{
				return &Map->Data[CurIdx].data;
			}
			bool operator==(const Iterator& comp) const
			{
				return (CurIdx == comp.CurIdx) && (Map == comp.Map);
			}
			bool operator!=(const Iterator& comp) const
			{
				return !((CurIdx == comp.CurIdx) && (Map == comp.Map));
			}
			Iterator& operator=(const Iterator& rhs)
			{
				Map = rhs.Map;
				CurIdx = rhs.CurIdx;
				return *this;
			}
			const _KeyType& GetKey()
			{
				return Map->Data[CurIdx].key;
			}
		private:
			atBinaryMap<_T, _KeyType>* Map;
			int CurIdx;
		};
		class ConstIterator
		{
		public:
			ConstIterator() : Map(NULL), CurIdx(0)
			{}
			explicit ConstIterator(const atBinaryMap<_T, _KeyType>& themap) :Map(&themap), CurIdx(0)
			{}
			ConstIterator& operator++()
			{
				if (++CurIdx >= Map->Data.GetCount())
					*this = Map->end();
				return *this;
			}
			ConstIterator operator++(const int)
			{
				ConstIterator result(*this);
				++(*this);
				return result;
			}
			const _T& operator*() const
			{
				return Map->Data[CurIdx].data;
			}
			const _T* operator->() const
			{
				return &Map->Data[CurIdx].data;
			}
			bool operator==(const ConstIterator& comp) const
			{
				return (CurIdx == comp.CurIdx) && (Map == comp.Map);
			}
			bool operator!=(const ConstIterator& comp) const
			{
				return !((CurIdx == comp.CurIdx) && (Map == comp.Map));
			}
			ConstIterator& operator=(const ConstIterator& rhs)
			{
				Map = rhs.Map;
				CurIdx = rhs.CurIdx;
				return *this;
			}
			const _KeyType& GetKey()
			{
				return Map->Data[CurIdx].key;
			}
		private:
			const atBinaryMap<_T, _KeyType>* Map;
			int CurIdx;
		};
		friend class Iterator;
		friend class ConstIterator;
		Iterator begin()
		{
			if (Data.size() > 0)
				return Iterator(*this);
			else
				return end();
		}
		Iterator end()
		{
			return Iterator();
		}
		ConstIterator end() const
		{
			return ConstIterator();
		}
		enum ePlaceNoneInitializer
		{
			PLACE_NONE
		};
		enum ePlaceKeyInitializer
		{
			PLACE_KEY
		};
		enum ePlaceDataInitializer
		{
			PLACE_DATA
		};
		enum ePlaceKeyDataInitializer
		{
			PLACE_KEY_AND_DATA
		};
		int GetCount() const
		{
			return Data.size();
		}
		_T* GetItem(int i)
		{
			return &Data[i].data;
		}
		const _T* GetItem(int i) const
		{
			return &Data[i].data;
		}
		_KeyType* GetKey(int i)
		{
			return &Data[i].key;
		}
		const _KeyType* GetKey(int i) const
		{
			return &Data[i].key;
		}
		struct DataPair
		{
			_KeyType key;
			_T data;
		};
	protected:
		bool Sorted;
		char Pad[3];
		atArray<DataPair> Data;
	};
	inline u32 atHash(unsigned x)
	{
		return x;
	}
	inline u32 atHash(const void* x)
	{
		return (u32)(u64)x;
	}
	inline u32 atHash_const_char(const char* s)
	{
		u32 h = 0, g;
		if (!s)
			return 0;
		// stolen from data/hash, presumably whoever wrote
		// this knew what they were doing.
		while (*s)
		{
			h = (h << 4) + (*s++);
			if (((g = h & 0xf0000000)) != 0)
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}
		return h;
	}
	inline u32 atHash_const_charU(const char* s)
	{
		u32 h = 0, g;
		while (*s)
		{
			char c = *s++;
			if (c >= 'a' && c <= 'z')
				c -= 0x20;
			h = (h << 4) + c;
			if (((g = h & 0xf0000000)) != 0)
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}
		return h;
	}
	inline u16 atHash16(const char* s)
	{
		u32 hash = atHash_const_char(s);
		u32 prime = 65167;
		return u16((hash % prime) + (0xffff - prime));
	}
	inline u16 atHash16U(const char* s)
	{
		u32 hash = atHash_const_charU(s);
		u32 prime = 65167;
		return u16((hash % prime) + (0xffff - prime));
	}
	inline u32 atHash64(u64 key)
	{
		u32 h = 0, g;
		u32 count = 8;
		while (--count)
		{
			char s = (char)key; // take the bottom 8 bits
			key = key >> 8;

			h = (h << 4) + s;
			if (((g = h & 0xf0000000)) != 0)
			{
				h = h ^ (g >> 24);
				h = h ^ g;
			}
		}

		return h;
	}
	template <typename T>
	struct atMapHashFn
	{
		u32 operator()(const T& key) const
		{
			return atHash(key);
		}
	};
	template <>
	struct atMapHashFn<const char*>
	{
		u32 operator ()(const char* key) const
		{
			return atHash_const_char(key);
		}
	};
	template <> struct atMapHashFn<atString> : public atMapHashFn<const char*>
	{};
	template <> struct atMapHashFn<ConstString> : public atMapHashFn<const char*>
	{};
	struct atMapCaseInsensitiveHashFn
	{
		u32 operator ()(const char* key) const
		{
			return atHash_const_charU(key);
		}
	};
	template <>
	struct atMapHashFn<u64>
	{
		u32 operator ()(u64 key) const
		{
			return (atHash64(key));
		}
	};
	template <typename _T>
	struct atMapEquals
	{
		bool operator ()(const _T& left, const _T& right) const
		{
			return left == right;
		}
	};
	template <>
	struct atMapEquals<const char*>
	{
		bool operator ()(const char* left, const char* right) const
		{
			return strcmp(left, right) == 0;
		}
	};
	template <> struct atMapEquals<atString> : public atMapEquals<const char*>
	{};
	template <> struct atMapEquals<ConstString> : public atMapEquals<const char*>
	{};
	template <>
	struct atMapEquals<u64>
	{
		bool operator ()(const u64 left, const u64 right) const
		{
			return (left == right);
		}
	};
	template <typename _Key, typename _Data>
	struct atMapEntry
	{
		atMapEntry(const _Key& k, atMapEntry* n) : key(k), data(), next(n)
		{}
		atMapEntry(const _Key& k, const _Data& d, atMapEntry* n) : key(k), data(d), next(n)
		{}
		_Key key;
		_Data data;
		atMapEntry* next;
		typedef atMapEntry<_Key, _Data> _ThisType;

		enum ePlaceNoneInitializer
		{
			PLACE_NONE
		};
		enum ePlaceKeyInitializer
		{
			PLACE_KEY
		};
		enum ePlaceDataInitializer
		{
			PLACE_DATA
		};
		enum ePlaceKeyDataInitializer
		{
			PLACE_KEY_AND_DATA
		};
	};
	template <typename _Key, typename _Data>
	class atMapMemory
	{
		typedef atMapEntry<_Key, _Data> _EntryType;
	public:
		_EntryType* Allocate(const _Key& k, _EntryType* n)
		{
			return rage_new _EntryType(k, n);
		}
		_EntryType* Allocate(const _Key& k, const _Data& d, _EntryType* n)
		{
			return rage_new _EntryType(k, d, n);
		}
		void DeAllocate(_EntryType* ptr)
		{
			delete ptr;
		}
		void AllocateHash(int slotCount, _EntryType**& hash)
		{
			hash = rage_new _EntryType * [slotCount];
		}
		void DeAllocateHash(_EntryType** hash)
		{
			delete[] hash;
		}
	};
	struct atMapCaseInsensitiveEquals
	{
		bool operator ()(const char* left, const char* right) const
		{
			return _stricmp(left, right) == 0;
		}
	};
	template <class _Key,class _Data,class _Hash = atMapHashFn<_Key>,class _Equals = atMapEquals<_Key>, class _MemoryPolicy = atMapMemory< _Key, _Data> >
	class atMap
	{
	public:
		typedef atMapEntry<_Key, _Data> Entry;
		typedef _Key KeyType;
		typedef _Data DataType;

		int GetNumSlots() const
		{
			return m_Slots;
		}
		int GetNumUsed() const
		{
			return m_Used;
		}
		Entry* GetEntry(int i)
		{
			return m_Hash[i];
		}
		const Entry* GetEntry(int i) const
		{
			return m_Hash[i];
		}

		class Iterator
		{
		private:
			friend class atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>;
			int hashpos;
			Entry* n;
			atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>* map;
			Iterator(atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>& m) : hashpos(0), n(0), map(&m)
			{
				Start();
			}
		public:
			void Start()
			{
				int localHashPos;
				int slotCount = map->GetNumSlots();
				for (localHashPos = 0; localHashPos < slotCount; localHashPos++)
				{
					n = map->GetEntry(localHashPos);
					if (n)
						break;
				}
				hashpos = localHashPos;
			}
			void Next()
			{
				if (AtEnd())
					return;
				n = n->next;
				int localHashPos = hashpos;
				const int numSlots = map->GetNumSlots();
				while (!n && (localHashPos + 1) < numSlots)
				{
					localHashPos++;
					n = map->GetEntry(localHashPos);
				}
				hashpos = localHashPos;
			}
			bool AtEnd() const
			{
				return !n;
			}
			_Key& GetKey() const
			{
				FastAssert(n); return n->key;
			}
			_Data& GetData() const
			{
				FastAssert(n); return n->data;
			}
			_Data* GetDataPtr() const
			{
				FastAssert(n); return &n->data;
			}
			_Data& operator*() const
			{
				return GetData();
			}
			_Data* operator->() const
			{
				FastAssert(n); return &n->data;
			}
			Iterator& operator++()
			{
				Next(); return *this;
			}
			operator bool() const
			{
				return !AtEnd();
			}
		};

		// PURPOSE: Class to encapsulate linear iteration through all inserted items in map, and to maintain const-only access to the items
		class ConstIterator
		{
		private:
			friend class atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>;
			int hashpos;
			const Entry* cn;
			const atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>* cmap;
			ConstIterator(const atMap<_Key, _Data, _Hash, _Equals, _MemoryPolicy>& m) : hashpos(0), cn(0), cmap(&m)
			{
				Start();
			}
		public:
			void Start()
			{
				for (hashpos = 0; hashpos < cmap->GetNumSlots(); hashpos++)
				{
					cn = cmap->GetEntry(hashpos);
					if (cn) break;
				}
			}
			void Next()
			{
				if (AtEnd()) return;
				cn = cn->next;
				const int numSlots = cmap->GetNumSlots();
				while (!cn && (hashpos + 1) < numSlots)
				{
					hashpos++;
					cn = cmap->GetEntry(hashpos);
				}
			}
			bool AtEnd() const
			{
				return !cn;
			}
			const _Key& GetKey() const
			{
				return cn->key;
			}
			const _Data& GetData() const
			{
				return cn->data;
			}
			const _Data* GetDataPtr() const
			{
				return &cn->data;
			}
			const _Data& operator*() const
			{
				return GetData();
			}
			const _Data* operator->() const
			{
				return &cn->data;
			}
			ConstIterator& operator ++ ()
			{
				Next(); return *this;
			}
			operator bool() const
			{
				return !AtEnd();
			}
		};
		Entry** m_Hash; // Storage for toplevel hash array
		u16 m_Slots; // Number of slots in toplevel hash
		u16 m_Used; // Number of those slots currently in use
		_Hash m_HashFn; // Hash functor
		_Equals m_Equals; // Equality functor
		_MemoryPolicy m_Memory;	 // Memory functor
		u8 m_AllowReCompute; //Make sure we can't do dynamic memory allocation (good for resourced maps and general dynamic memory use removal)
	};
	template <typename K, typename T>
	class inmap_node
	{
	public:
		inmap_node()
			: m_right(0)
			, m_left(0)
			, m_parent(0)
		{}

		//Copy constructor intentionally left empty.
		inmap_node(const inmap_node<K, T>&)
			: m_right(0)
			, m_left(0), m_parent(0)
		{}
		//Assigment operator intentionally left empty.
		inmap_node<K, T>& operator=(const inmap_node<K, T>&)
		{
			return *this;
		}
		// WARNING!!!! Be careful when changing the size of inmap_nodes
		// inmap_nodes are used in resourced objects, though they themselves don't
		// get fixed up.
		// Changing the size will likely mean having to rebuild resources.
		T* m_right;
		T* m_left;
		T* m_parent;
		K m_key;
	};
	template <typename K, typename T, inmap_node<K, T> T::*NODE>
	struct inmap_detail
	{
		//Store the node color as the LSB of one of the pointers.
		//Use TProxy and CProxy (color proxy) to proxy a T* that has might
		//have its LSB set.
		struct TProxy
		{
			explicit TProxy(T*& t)
				: m_T(t)
			{}
			operator T* ()
			{
				return (T*)(u64(m_T) & ~u64(0x01));
			}
			TProxy& operator=(T* t)
			{
				m_T = (T*)(u64(t) | (u64(m_T) & u64(0x01)));
				return *this;
			}
			TProxy& operator=(TProxy& that)
			{
				if (this != &that)
				{
					m_T = that.m_T;
				}
				return *this;
			}
			T*& m_T;
		};
		struct CProxy
		{
			explicit CProxy(T*& t)
				: m_T(t)
			{}
			operator bool()
			{
				return bool(u64(m_T) & 0x01);
			}
			CProxy& operator=(const bool color)
			{
				m_T = (T*)(u64(color) | (u64(m_T) & ~u64(0x01)));
				return *this;
			}
			CProxy& operator=(CProxy& that)
			{
				if (this != &that)
				{
					m_T = that.m_T;
				}
				return *this;
			}
			T*& m_T;
		};
		static TProxy right(T* t)
		{
			return TProxy((t->*NODE).m_right);
		}
		//static T*& right( T* t ) { return ( t->*NODE ).m_right; }
		//static constexpr T* right( const T* t ) { return ( t->*NODE ).m_right; }
		static T*& left(T* t)
		{
			return (t->*NODE).m_left;
		}
		//static constexpr T* left( const T* t ) { return ( t->*NODE ).m_left; }
		static T*& parent(T* t)
		{
			return (t->*NODE).m_parent;
		}
		//static constexpr T* parent( const T* t ) { return ( t->*NODE ).m_parent; }
		static T*& grandparent(T* t)
		{
			return parent(parent(t));
		}
		//static constexpr T* grandparent( const T* t ) { return parent( parent( t ) ); }
		static T*& uncle(T* t)
		{
			T* p = parent(t);
			T* gp = parent(p);
			T* lu = left(gp);
			return p == lu ? right(gp) : lu;
		}
		//static constexpr T* uncle( const T* t ) { return uncle( const_cast< T* >( t ) ); }
		static CProxy color(T* t)
		{
			return CProxy((t->*NODE).m_right);
		}
		//static bool& color( T* t ) { return ( t->*NODE ).m_color; }
		//static bool color( const T* t ) { return ( t->*NODE ).m_color; }
		static K& key(T* t)
		{
			return (t->*NODE).m_key;
		}
		static K key(const T* t)
		{
			return (t->*NODE).m_key;
		}
		static T* minimum(T* t)
		{
			for (T* l = left(t); l; t = l, l = left(t))
			{
			}
			return t;
		}
		static constexpr T* minimum(const T* t)
		{
			return minimum(const_cast<T*>(t));
		}
		static T* maximum(T* t)
		{
			for (T* r = right(t); r; t = r, r = right(t))
			{
			}
			return t;
		}
		static constexpr T* maximum(const T* t)
		{
			return maximum(const_cast<T*>(t));
		}
		static T* predecessor(T* t)
		{
			T* p = left(t);
			if (p)
			{
				p = maximum(p);
			}
			else
			{
				T* s = t;
				p = parent(s);
				while (p && s == left(p))
				{
					s = p;
					p = parent(p);
				}
			}
			return p;
		}
		static constexpr T* predecessor(const T* t)
		{
			return predecessor(const_cast<T*>(t));
		}
		static T* successor(T* t)
		{
			T* s = right(t);
			if (s)
			{
				s = minimum(s);
			}
			else
			{
				T* p = t;
				s = parent(p);
				while (s && p == right(s))
				{
					p = s;
					s = parent(s);
				}
			}
			return s;
		}
		static constexpr T* successor(const T* t)
		{
			return successor(const_cast<T*>(t));
		}
		//PURPOSE
		//  Template we'll use to create reverse_iterators from iterators
		template <typename ITER>
		class reverse_iterator
		{
		public:
			typedef typename ITER::iterator_category iterator_category;
			typedef typename ITER::value_type value_type;
			typedef typename ITER::difference_type difference_type;
			typedef typename ITER::pointer pointer;
			typedef typename ITER::reference reference;
			reverse_iterator()
			{}
			explicit reverse_iterator(const ITER& it)
				: m_It(it)
			{}
			template< typename OTHER >
			reverse_iterator(const reverse_iterator< OTHER >& that)
				: m_It(that.base())
			{}
			template< typename OTHER >
			reverse_iterator< ITER >& operator=(const reverse_iterator< OTHER >& that)
			{
				m_It = that.base();
				return *this;
			}
			ITER base() const
			{
				return m_It;
			}
			reverse_iterator operator++()
			{
				--m_It;
				return *this;
			}
			reverse_iterator operator--()
			{
				++m_It;
				return *this;
			}
			reverse_iterator operator++(const int)
			{
				reverse_iterator tmp = *this;
				--m_It;
				return tmp;
			}
			reverse_iterator operator--(const int)
			{
				reverse_iterator tmp = *this;
				++m_It;
				return tmp;
			}
			pointer operator->() const
			{
				ITER tmp = m_It;
				return (--tmp).operator->();
			}
			reference operator*() const
			{
				ITER tmp = m_It;
				return (--tmp).operator*();
			}
			bool operator==(const reverse_iterator& rhs) const
			{
				return m_It == rhs.m_It;
			}
			bool operator!=(const reverse_iterator& rhs) const
			{
				return m_It != rhs.m_It;
			}
		private:
			ITER m_It;
		};
	}; //struct inmap_detail
	template <typename K, typename T, inmap_node<K, T> T::*NODE, typename P>
	class inmap_base
	{
		typedef inmap_base<K, T, NODE, P> MapType;
		typedef inmap_detail<K, T, NODE> detail;
	public:
		enum
		{
			ALLOW_DUP_KEYS = P::ALLOW_DUP_KEYS
		};
		class const_iterator;
		class iterator
		{
			friend class inmap_base<K, T, NODE, P>;
			friend class const_iterator;
			bool before_begin() const
			{
				const u64 one = 0x1;
				return (((u64)m_map) & ~one) && (((u64)m_map) & one);
			}
			void set_before_begin()
			{
				const u64 one = 0x1;
				m_map = (MapType*)(((u64)m_map) | one);
			}
			void clear_before_begin()
			{
				const u64 one = 0x1;
				m_map = (MapType*)(((u64)m_map) & ~one);
			}
			MapType* map()
			{
				const u64 one = 0x1;
				return (MapType*)(((u64)m_map) & ~one);
			}
			MapType* map() const
			{
				const u64 one = 0x1;
				return (MapType*)(((u64)m_map) & ~one);
			}
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::pair<K, T*> value_type;
			typedef ptrdiff_t difference_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			//Can't make this a true reference - the intrusive nature
			//of the list prevents us from changing the value of a contained item.
			typedef value_type reference;
			iterator() : m_map(0)
			{
				m_Value.second = 0;
			}
			iterator operator++()
			{
				if (m_Value.second)
				{
					m_Value.second = detail::successor(m_Value.second);
				}
				else if (this->before_begin())
				{
					//We're one before begin - go to begin
					this->clear_before_begin();
					m_Value.second = detail::minimum(m_map->m_root);
				}
				if (m_Value.second)
				{
					m_Value.first = detail::key(m_Value.second);
				}
				return *this;
			}
			iterator operator--()
			{
				if (m_Value.second)
				{
					m_Value.second = detail::predecessor(m_Value.second);
					if (!m_Value.second)
					{
						//Went past begin.
						this->set_before_begin();
					}
				}
				else if (m_map && !this->before_begin())
				{
					//We're at the end - go to one before the end
					m_Value.second = detail::maximum(m_map->m_root);
				}
				if (m_Value.second)
				{
					m_Value.first = detail::key(m_Value.second);
				}
				return *this;
			}
			iterator operator++(const int)
			{
				iterator tmp = *this;
				++* this;
				return tmp;
			}
			iterator operator--(const int)
			{
				iterator tmp = *this;
				--* this;
				return tmp;
			}
			pointer operator->()
			{
				return &m_Value;
			}
			const_pointer operator->() const
			{
				return &m_Value;
			}
			reference operator*() const
			{
				return m_Value;
			}
			bool operator==(const iterator& rhs) const
			{
				return rhs.m_Value.second == m_Value.second;
			}
			bool operator!=(const iterator& rhs) const
			{
				return rhs.m_Value.second != m_Value.second;
			}
			bool operator==(const const_iterator& rhs) const
			{
				return rhs == *this;
			}
			bool operator!=(const const_iterator& rhs) const
			{
				return rhs != *this;
			}
		private:
			iterator(T* t, MapType* m) : m_map(m)
			{
				if (t)
				{
					m_Value.first = detail::key(t);
					m_Value.second = t;
				}
				else
				{
					m_Value.second = 0;
				}
			}
			value_type m_Value;
			MapType* m_map;
		};
		class const_iterator
		{
			friend class inmap_base<K, T, NODE, P>;
			bool before_begin() const
			{
				return (((u64)m_map) & ~0x01u) && (((u64)m_map) & 0x01u);
			}
			void set_before_begin()
			{
				m_map = (MapType*)(((u64)m_map) | 0x01u);
			}
			void clear_before_begin()
			{
				m_map = (MapType*)(((u64)m_map) & ~0x01u);
			}
			MapType* map()
			{
				return (MapType*)(((u64)m_map) & ~0x01u);
			}
			MapType* map() const
			{
				return (MapType*)(((u64)m_map) & ~0x01u);
			}
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef std::pair<K, const T*> value_type;
			typedef ptrdiff_t difference_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			//Can't make this a true reference - the intrusive nature
			//of the list prevents us from changing the value of a contained item.
			typedef value_type reference;
			const_iterator()
				: m_map(0)
			{
				m_Value.second = 0;
			}
			const_iterator(const iterator& it)
				: m_map(it.m_map)
			{
				this->m_Value.first = it.m_Value.first;
				this->m_Value.second = it.m_Value.second;
			}
			const_iterator operator++()
			{
				if (m_Value.second)
				{
					m_Value.second = detail::successor(m_Value.second);
				}
				else if (this->before_begin())
				{
					//We're one before begin - go to begin
					this->clear_before_begin();
					m_Value.second = detail::minimum(m_map->m_root);
				}
				if (m_Value.second)
				{
					m_Value.first = detail::key(m_Value.second);
				}
				return *this;
			}
			const_iterator operator--()
			{
				if (m_Value.second)
				{
					m_Value.second = detail::predecessor(m_Value.second);
					if (!m_Value.second)
					{
						//Went past begin.
						this->set_before_begin();
					}
				}
				else if (m_map && !this->before_begin())
				{
					//We're at the end - go to one before the end
					m_Value.second = detail::maximum(m_map->m_root);
				}
				if (m_Value.second)
				{
					m_Value.first = detail::key(m_Value.second);
				}
				return *this;
			}
			const_iterator operator++(const int)
			{
				const_iterator tmp = *this;
				++* this;
				return tmp;
			}
			const_iterator operator--(const int)
			{
				const_iterator tmp = *this;
				--* this;
				return tmp;
			}
			pointer operator->()
			{
				return &m_Value;
			}
			const_pointer operator->() const
			{
				return &m_Value;
			}
			reference operator*() const
			{
				return m_Value;
			}
			bool operator==(const const_iterator& rhs) const
			{
				return rhs.m_Value.second == m_Value.second;
			}
			bool operator!=(const const_iterator& rhs) const
			{
				return rhs.m_Value.second != m_Value.second;
			}
		private:
			const_iterator(T* t, const MapType* m) : m_map(m)
			{
				if (t)
				{
					m_Value.first = detail::key(t);
					m_Value.second = t;
				}
				else
				{
					m_Value.second = 0;
				}
			}
			value_type m_Value;
			const MapType* m_map;
		};
		typedef typename detail::template reverse_iterator<iterator> reverse_iterator;
		typedef typename detail::template reverse_iterator<const_iterator> const_reverse_iterator;
		typedef std::pair<K, T*> value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef T* mapped_type;
		typedef K key_type;
		typedef u64 u64ype;
		typedef ptrdiff_t difference_type;
		typedef typename P::Predicate key_compare;
		struct value_compare
		{
			bool operator()(const value_type& v0, const value_type& v1)
			{
				P pred;
				return pred(v0.first, v1.first);
			}
		};
		inmap_base()
			: m_root(0), m_size(0)
		{}
		iterator begin()
		{
			return iterator(m_root ? minimum(m_root) : 0, this);
		}
		const_iterator begin() const
		{
			return const_iterator(m_root ? minimum(m_root) : 0, this);
		}
		iterator end()
		{
			return iterator(0, this);
		}
		const_iterator end() const
		{
			return const_iterator(0, this);
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(this->end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(this->end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(this->begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(this->begin());
		}
		void clear()
		{
			while (!this->empty())
			{
				this->erase(this->begin());
			}
		}
		u64ype count(const K& key) const
		{
			const_iterator it = this->find(key);
			const_iterator stop = this->end();
			int n = 0;
			for (; stop != it && key == it->first; ++it)
			{
				++n;
			}
			return n;
		}
		bool empty() const
		{
			return !m_size;
		}
		std::pair<iterator, bool> insert(const value_type& item)
		{
			return this->insert(item.first, item.second);
		}
		std::pair<iterator, bool> insert(const K& k, T* item)
		{
			T* n = 0;
			if (!m_root)
			{
				n = m_root = item;
			}
			else
			{
				T* cur = m_root;
				while (cur)
				{
					T* next;
					if (m_pred(k, key(cur)))
					{
						next = left(cur);
						if (!next)
						{
							left(cur) = item;
							parent(item) = cur;
							n = item;
						}
					}
					//If allowing duplicates, items with equal keys go right.
					else if (m_pred(key(cur), k) || ALLOW_DUP_KEYS)
					{
						next = right(cur);
						if (!next)
						{
							right(cur) = item;
							parent(item) = cur;
							n = item;
						}
					}
					else
					{
						break;
					}
					cur = next;
				}
			}
			if (n)
			{
				color(item) = true;
				insertFixup(item);
				key(item) = k;
				++m_size;
			}
			return std::pair<iterator, bool>(iterator(n, this), 0 != n);
		}
		key_compare key_comp() const
		{
			return m_pred;
		}
		iterator find(const K& k, u64* cost = 0)
		{
			iterator it = this->lower_bound(k, cost);
			return (this->end() == it || m_pred(k, it->first)) ? this->end() : it;
		}
		const_iterator find(const K& k, u64* cost = 0) const
		{
			const_iterator it = this->lower_bound(k, cost);
			return (this->end() == it || m_pred(k, it->first)) ? this->end() : it;
		}
		iterator erase(T* item)
		{
			iterator next(item, this);
			++next;
			T* y;
			if (!left(item) || !right(item))
			{
				y = item;
			}
			else if (m_size & 0x01)
			{
				y = right(item);
				while (left(y)) y = left(y);
			}
			else
			{
				y = left(item);
				while (right(y)) y = right(y);
			}
			T* x;
			if (left(y))
			{
				x = left(y);
			}
			else
			{
				x = right(y);
			}
			if (x)
			{
				parent(x) = parent(y);
			}
			if (!parent(y))
			{
				m_root = x;
			}
			else if (left(parent(y)) == y)
			{
				left(parent(y)) = x;
			}
			else
			{
				right(parent(y)) = x;
			}
			if (item != y)
			{
				if (item == m_root)
				{
					m_root = y;
				}
				else if (left(parent(item)) == item)
				{
					left(parent(item)) = y;
				}
				else
				{
					right(parent(item)) = y;
				}
				parent(y) = parent(item);
				left(y) = left(item);
				right(y) = right(item);
				color(y) = color(item);
				if (left(item))
				{
					parent(left(item)) = y;
				}
				if (right(item))
				{
					parent(right(item)) = y;
				}
			}
			if (x && color(y) == false)
			{
				this->eraseFixup(x);
			}
			//Use the raw pointer here, not the accessors.
			//Using the accessors can leave the bit zero set to 1, which
			//will compare not equal to NULL, and we don't want that.
			(item->*NODE).m_parent =
				(item->*NODE).m_left =
				(item->*NODE).m_right = 0;
			--m_size;
			return next;
		}
		iterator erase(iterator where)
		{
			return where->second ? this->erase(where->second) : this->end();
		}
		iterator erase(iterator first, iterator last)
		{
			if (this->begin() == first && this->end() == last)
			{
				this->clear();
				first = this->end();
			}
			else
			{
				while (first != last)
				{
					first = this->erase(first);
				}
			}
			return first;
		}
		u64ype erase(const K& k)
		{
			u64 count = 0;
			iterator it = this->find(k);
			const_iterator stop = this->end();
			while (stop != it && k == it->first)
			{
				it = this->erase(it);
				++count;
			}
			return count;
		}
		std::pair<iterator, iterator> equal_range(const K& k)
		{
			return std::pair<iterator, iterator>(this->lower_bound(k), this->upper_bound(k));
		}
		std::pair<const_iterator, const_iterator> equal_range(const K& k) const
		{
			return std::pair<const_iterator, const_iterator>(this->lower_bound(k), this->upper_bound(k));
		}
		iterator lower_bound(const K& k, u64* cost = 0)
		{
			T* cur = m_root;
			T* n = 0;
			u64 findCost = 0;
			while (cur)
			{
				if (m_pred(key(cur), k))
				{
					cur = right(cur);
				}
				else
				{
					n = cur;
					cur = left(cur);
				}
				++findCost;
			}
			if (ALLOW_DUP_KEYS && n)
			{
				cur = predecessor(n);
				while (cur
					&& !m_pred(key(cur), k)
					&& !m_pred(k, key(cur)))
				{
					n = cur;
					cur = predecessor(cur);
					++findCost;
				}
			}
			if (cost)
				*cost = findCost;
			return iterator(n, this);
		}
		const_iterator lower_bound(const K& k, u64* cost = 0) const
		{
			return const_cast<MapType*>(this)->lower_bound(k, cost);
		}
		iterator upper_bound(const K& k, u64* cost = 0)
		{
			T* cur = m_root;
			T* n = 0;
			u64 findCost = 0;
			while (cur)
			{
				if (m_pred(k, key(cur)))
				{
					n = cur;
					cur = left(cur);
				}
				else
				{
					cur = right(cur);
				}
				++findCost;
			}
			if (cost)
				*cost = findCost;
			return iterator(n, this);
		}
		const_iterator upper_bound(const K& k, u64* cost = 0) const
		{
			return const_cast<MapType*>(this)->upper_bound(k, cost);
		}
		u64ype max_size() const
		{
			const u64ype maxsize = u64ype(-1) >> 1;
			return (0 < maxsize ? maxsize : 1);
		}
		u64ype size() const
		{
			return m_size;
		}
		value_compare value_comp() const
		{
			value_compare vc;
			return vc;
		}
	private:
		void insertFixup(T* x)
		{
			/* check Red-Black properties */
			while (x != m_root && color(parent(x)) == true)
			{
				/* we have a violation */
				if (parent(x) == left(grandparent(x)))
				{
					T* y = right(grandparent(x));
					if (y && color(y) == true)
					{
						/* uncle is RED */
						color(parent(x)) = false;
						color(y) = false;
						color(grandparent(x)) = true;
						x = grandparent(x);
					}
					else
					{
						/* uncle is BLACK */
						if (x == right(parent(x)))
						{
							/* make x a left child */
							x = parent(x);
							rotl(x);
						}
						/* recolor and rotate */
						color(parent(x)) = false;
						color(grandparent(x)) = true;
						rotr(grandparent(x));
					}
				}
				else
				{
					/* mirror image of above code */
					T* y = left(grandparent(x));
					if (y && color(y) == true)
					{
						/* uncle is RED */
						color(parent(x)) = false;
						color(y) = false;
						color(grandparent(x)) = true;
						x = grandparent(x);
					}
					else
					{
						/* uncle is BLACK */
						if (x == left(parent(x)))
						{
							x = parent(x);
							rotr(x);
						}
						color(parent(x)) = false;
						color(grandparent(x)) = true;
						rotl(grandparent(x));
					}
				}
			}
			color(m_root) = false;
		}
		void eraseFixup(T* x)
		{
			while (x != m_root && color(x) == false)
			{
				if (x == left(parent(x)))
				{
					T* w = right(parent(x));
					if (w && color(w) == true)
					{
						color(w) = false;
						color(parent(x)) = true;
						rotl(parent(x));
						w = right(parent(x));
					}
					if (!w)
					{
						x = parent(x);
						continue;
					}
					if ((!left(w) || color(left(w)) == false)
						&& (!right(w) || color(right(w)) == false))
					{
						color(w) = true;
						x = parent(x);
					}
					else
					{
						if (!right(w) || color(right(w)) == false)
						{
							color(left(w)) = false;
							color(w) = true;
							rotr(w);
							w = right(parent(x));
						}
						color(w) = color(parent(x));
						color(parent(x)) = false;
						color(right(w)) = false;
						rotl(parent(x));
						x = m_root;
					}
				}
				else
				{
					T* w = left(parent(x));
					if (w && color(w) == true)
					{
						color(w) = false;
						color(parent(x)) = true;
						rotr(parent(x));
						w = left(parent(x));
					}
					if (!w)
					{
						x = parent(x);
						continue;
					}
					if ((!right(w) || color(right(w)) == false)
						&& (!left(w) || color(left(w)) == false))
					{
						color(w) = true;
						x = parent(x);
					}
					else
					{
						if (!left(w) || color(left(w)) == false)
						{
							color(right(w)) = false;
							color(w) = true;
							rotl(w);
							w = left(parent(x));
						}
						color(w) = color(parent(x));
						color(parent(x)) = false;
						color(left(w)) = false;
						rotr(parent(x));
						x = m_root;
					}
				}
			}
			color(x) = false;
		}
		void rotl(T* t)
		{
			T* n = right(t);
			right(t) = left(n);
			if (left(n))
			{
				parent(left(n)) = t;
			}
			parent(n) = parent(t);
			if (t == m_root)
			{
				m_root = n;
			}
			else if (t == left(parent(t)))
			{
				left(parent(t)) = n;
			}
			else
			{
				right(parent(t)) = n;
			}
			left(n) = t;
			parent(t) = n;
		}
		void rotr(T* t)
		{
			T* n = left(t);
			left(t) = right(n);
			if (right(n))
			{
				parent(right(n)) = t;
			}
			parent(n) = parent(t);
			if (t == m_root)
			{
				m_root = n;
			}
			else if (t == right(parent(t)))
			{
				right(parent(t)) = n;
			}
			else
			{
				left(parent(t)) = n;
			}
			right(n) = t;
			parent(t) = n;
		}
		static typename detail::TProxy right(T* t)
		{
			return detail::right(t);
		}
		static T*& left(T* t)
		{
			return detail::left(t);
		}
		static T*& parent(T* t)
		{
			return detail::parent(t);
		}
		static T*& grandparent(T* t)
		{
			return detail::grandparent(t);
		}
		static T*& uncle(T* t)
		{
			return detail::uncle(t);
		}
		static K& key(T* t)
		{
			return detail::key(t);
		}
		static typename detail::CProxy color(T* t)
		{
			return detail::color(t);
		}
		static T* minimum(T* t)
		{
			return detail::minimum(t);
		}
		static T* maximum(T* t)
		{
			return detail::maximum(t);
		}
		static T* predecessor(T* t)
		{
			return detail::predecessor(t);
		}
		static T* successor(T* t)
		{
			return detail::successor(t);
		}
		T* m_root;
		u64ype m_size;
		key_compare m_pred;
		//Non-copyable
		inmap_base(const inmap_base<K, T, NODE, P >&);
		inmap_base& operator=(const inmap_base<K, T, NODE, P>&);
	};
	template <typename PRED>
	struct inmap_policies
	{
		typedef PRED Predicate;
		enum
		{
			ALLOW_DUP_KEYS = false
		};
	};
	template <typename PRED>
	struct inmultimap_policies
	{
		typedef PRED Predicate;
		enum
		{
			ALLOW_DUP_KEYS = true
		};
	};
	template <typename K, typename T, inmap_node<K, T> T::*NODE, typename P = std::less<K>>
	class inmap : public inmap_base<K, T, NODE, inmap_policies<P>>
	{
	public:
		inmap()
		{}
	private:
		//Non-copyable
		inmap(const inmap< K, T, NODE, P >&);
		inmap& operator=(const inmap< K, T, NODE, P >&);
	};
	template <typename K, typename T, inmap_node<K, T> T::*NODE, typename P = std::less<K>>
	class inmultimap : public inmap_base< K, T, NODE, inmultimap_policies< P > >
	{
	public:
		inmultimap()
		{}
	private:
		//Non-copyable
		inmultimap(const inmultimap< K, T, NODE, P >&);
		inmultimap& operator=(const inmultimap< K, T, NODE, P >&);
	};
	namespace inlist_detail
	{
		template <typename ITER>
		class reverse_iterator
		{
		public:
			typedef typename ITER::iterator_category iterator_category;
			typedef typename ITER::value_type value_type;
			typedef typename ITER::difference_type difference_type;
			typedef typename ITER::pointer pointer;
			typedef typename ITER::reference reference;
			reverse_iterator()
			{}
			explicit reverse_iterator(const ITER& it)
				: m_It(it)
			{}
			template <typename OTHER>
			reverse_iterator(const reverse_iterator<OTHER>& that)
				: m_It(that.base())
			{}
			template <typename OTHER>
			reverse_iterator<ITER>& operator=(const reverse_iterator<OTHER>& that)
			{
				m_It = that.base();
				return *this;
			}
			ITER base() const
			{
				return m_It;
			}
			reverse_iterator operator++()
			{
				--m_It;
				return *this;
			}
			reverse_iterator operator--()
			{
				++m_It;
				return *this;
			}
			reverse_iterator operator++(const int)
			{
				reverse_iterator tmp = *this;
				--m_It;
				return tmp;
			}
			reverse_iterator operator--(const int)
			{
				reverse_iterator tmp = *this;
				++m_It;
				return tmp;
			}
			pointer operator->() const
			{
				ITER tmp = m_It;
				return (--tmp).operator->();
			}
			reference operator*() const
			{
				ITER tmp = m_It;
				return (--tmp).operator*();
			}
			bool operator==(const reverse_iterator& rhs) const
			{
				return m_It == rhs.m_It;
			}
			bool operator!=(const reverse_iterator& rhs) const
			{
				return m_It != rhs.m_It;
			}
		private:
			ITER m_It;
		};
	}
	template<typename T>
	class inlist_node {
	public:
		inlist_node()
			: m_next(0) , m_prev(0)
		{}
		//Copy constructor intentionally left empty.
		inlist_node(const inlist_node<T>&)
			: m_next(0), m_prev(0)
		{}
		//Assignment operator intentionally left empty.
		inlist_node<T>& operator=(const inlist_node<T>&) {
			return *this;
		}
		T* m_next;
		T* m_prev;
	};
	template <typename T, inlist_node<T> T::*NODE>
	class inlist
	{
		typedef inlist<T, NODE> ListType;
	public:
		class const_iterator;
		class iterator
		{
			friend class inlist<T, NODE>;
			friend class const_iterator;
			bool before_begin() const
			{
				return (((u64)m_list) & ~0x01) && (((u64)m_list) & 0x01);
			}
			void set_before_begin()
			{
				m_list = (ListType*)(((u64)m_list) | 0x01);
			}
			void clear_before_begin()
			{
				m_list = (ListType*)(((u64)m_list) & ~0x01);
			}
			ListType* list()
			{
				return (ListType*)(((u64)m_list) & ~0x01);
			}
			ListType* list() const
			{
				return (ListType*)(((u64)m_list) & ~0x01);
			}
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef T* value_type;
			typedef ptrdiff_t difference_type;
			typedef value_type pointer;
			typedef const value_type* const_pointer;
			typedef value_type reference;
			iterator()
				: m_t(0), m_list(0)
			{}
			iterator operator++()
			{
				if (m_t)
				{
					m_t = (m_t->*NODE).m_next;
				}
				else if (this->before_begin())
				{
					this->clear_before_begin();
					m_t = m_list->m_head;
				}
				return *this;
			}
			iterator operator--()
			{
				if (m_t)
				{
					m_t = (m_t->*NODE).m_prev;
					if (!m_t)
					{
						this->set_before_begin();
					}
				}
				else if (m_list && !this->before_begin())
				{
					m_t = m_list->m_tail;
				}
				return *this;
			}
			iterator operator++(const int)
			{
				iterator tmp = *this;
				++* this;
				return tmp;
			}
			iterator operator--(const int)
			{
				iterator tmp = *this;
				--* this;
				return tmp;
			}
			pointer operator->()
			{
				return m_t;
			}
			const_pointer operator->() const
			{
				return m_t;
			}
			reference operator*() const
			{
				return m_t;
			}
			bool operator==(const iterator& rhs) const
			{
				return rhs.m_t == m_t;
			}
			bool operator!=(const iterator& rhs) const
			{
				return rhs.m_t != m_t;
			}
			bool operator==(const const_iterator& rhs) const
			{
				return rhs == *this;
			}
			bool operator!=(const const_iterator& rhs) const
			{
				return rhs != *this;
			}
		private:
			iterator(T* t, inlist<T, NODE>* l)
				: m_t(t), m_list(l)
			{}
			T* m_t;
			inlist<T, NODE>* m_list;
		};
		class const_iterator
		{
			friend class inlist<T, NODE>;
			bool before_begin() const
			{
				return (((u64)m_list) & ~0x01) && (((u64)m_list) & 0x01);
			}
			void set_before_begin()
			{
				m_list = (ListType*)(((u64)m_list) | 0x01);
			}
			void clear_before_begin()
			{
				m_list = (ListType*)(((u64)m_list) & ~0x01);
			}
			ListType* list()
			{
				return (ListType*)(((u64)m_list) & ~0x01);
			}
			ListType* list() const
			{
				return (ListType*)(((u64)m_list) & ~0x01);
			}
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef const T* value_type;
			typedef ptrdiff_t difference_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type reference;
			const_iterator()
				: m_t(0), m_list(0)
			{}
			const_iterator(const iterator& it)
				: m_t(it.m_t), m_list(it.m_list)
			{}
			const_iterator operator++()
			{
				if (m_t)
				{
					m_t = (m_t->*NODE).m_next;
				}
				else if (this->before_begin())
				{
					this->clear_before_begin();
					m_t = m_list->m_head;
				}
				return *this;
			}
			const_iterator operator--()
			{
				if (m_t)
				{
					m_t = (m_t->*NODE).m_prev;
					if (!m_t)
					{
						this->set_before_begin();
					}
				}
				else if (m_list && !this->before_begin())
				{
					m_t = m_list->m_tail;
				}
				return *this;
			}
			const_iterator operator++(const int)
			{
				const_iterator tmp = *this;
				++* this;
				return tmp;
			}
			const_iterator operator--(const int)
			{
				const_iterator tmp = *this;
				--* this;
				return tmp;
			}
			pointer operator->()
			{
				return m_t;
			}
			const_pointer operator->() const
			{
				return m_t;
			}
			reference operator*() const
			{
				return m_t;
			}
			bool operator==(const const_iterator& rhs) const
			{
				return rhs.m_t == m_t;
			}
			bool operator!=(const const_iterator& rhs) const
			{
				return rhs.m_t != m_t;
			}
		private:
			const_iterator(const T* t, const inlist<T, NODE>* l)
				: m_t(t), m_list(l)
			{}
			const T* m_t;
			const inlist<T, NODE>* m_list;
		};
		typedef typename inlist_detail::template reverse_iterator< iterator > reverse_iterator;
		typedef typename inlist_detail::template reverse_iterator< const_iterator > const_reverse_iterator;
		typedef typename iterator::reference reference;
		typedef typename const_iterator::reference const_reference;
		typedef typename iterator::pointer pointer;
		typedef typename const_iterator::pointer const_pointer;
		typedef typename iterator::value_type value_type;
		typedef typename iterator::difference_type difference_type;
		inlist()
			: m_head(0), m_tail(0), m_size(0)
		{}
		~inlist()
		{
			this->clear();
		}
		void push_front(T* item)
		{
			this->insert(this->begin(), item);
		}
		void push_back(T* item)
		{
			this->insert(this->end(), item);
		}
		iterator insert(iterator where, T* item)
		{
			return this->insert(where.m_t, item);
		}
		iterator insert(T* where, T* item)
		{
			if (0 == where)
			{
				if (m_tail)
				{
					next(m_tail) = item;
					prev(item) = m_tail;
					m_tail = item;
					if (!m_head) m_head = item;
				}
				else
				{
					m_head = m_tail = item;
				}
			}
			else if (where == m_head)
			{
				next(item) = m_head;
				prev(m_head) = item;
				m_head = item;
			}
			else
			{
				next(item) = where;
				prev(item) = prev(where);
				next(prev(where)) = item;
				prev(where) = item;
			}
			++m_size;
			return iterator(item, this);
		}
		void pop_front()
		{
			if (m_head)
			{
				this->erase(this->front());
			}
		}
		void pop_back()
		{
			if (m_tail)
			{
				this->erase(this->back());
			}
		}
		reference front()
		{
			return m_head;;
		}
		const_reference front() const
		{
			return m_head;
		}
		reference back()
		{
			return m_tail;
		}
		const_reference back() const
		{
			return m_tail;
		}
		iterator begin()
		{
			return iterator(m_head, this);
		}
		const_iterator begin() const
		{
			return const_iterator(m_head, this);
		}
		iterator end()
		{
			return iterator(0, this);
		}
		const_iterator end() const
		{
			return const_iterator(0, this);
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(this->end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(this->end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(this->begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(this->begin());
		}
		void clear()
		{
			while (!this->empty())
			{
				this->pop_front();
			}
		}
		bool empty() const
		{
			return !m_size;
		}
		iterator erase(iterator where)
		{
			iterator next = where;
			++next;
			this->erase(*where);
			return next;
		}
		iterator erase(iterator first, iterator last)
		{
			if (this->begin() == first && this->end() == last)
			{
				this->clear();
				first = this->end();
			}
			else
			{
				while (first != last)
				{
					first = this->erase(first);
				}
			}
			return first;
		}
		void erase(T* item)
		{
			if (item == m_head)
			{
				m_head = next(m_head);
				next(item) = 0;
				if (m_head)
				{
					prev(m_head) = 0;
				}
				else
				{
					m_tail = 0;
				}
			}
			else if (item == m_tail)
			{
				m_tail = prev(m_tail);
				prev(item) = 0;
				if (m_tail)
				{
					next(m_tail) = 0;
				}
			}
			else
			{
				next(prev(item)) = next(item);
				prev(next(item)) = prev(item);
				prev(item) = next(item) = 0;
			}
			--m_size;
		}
		bool is_present_unsafe(T* item) const
		{
			return  prev(item) || next(item) || item == m_head;
		}
		bool try_erase_unsafe(T* item)
		{
			if (is_present_unsafe(item))
			{
				erase(item);
				return true;
			}
			return false;
		}
		u64 size() const
		{
			return m_size;
		}
		u64 max_size() const
		{
			return u64(-1);
		}
		template <typename Pred>
		void merge(inlist<T, NODE>& rhs, Pred pred)
		{
			if (&rhs != this)
			{
				iterator it0 = this->begin();
				iterator stop0 = this->end();
				iterator it1 = rhs.begin();
				iterator stop1 = rhs.end();
				while (it0 != stop0 && it1 != stop1)
				{
					if (pred(*it1, *it0))
					{
						T* item1 = it1.m_t;
						it1 = rhs.erase(it1);
						this->insert(it0, item1);
					}
					else
					{
						++it0;
					}
				}
				while (it1 != stop1)
				{
					T* item1 = it1.m_t;
					it1 = rhs.erase(it1);
					this->insert(stop0, item1);
				}
			}
		}
	private:
		struct Less
		{
			bool operator()(const T* t0, const T* t1)
			{
				return t0 < t1;
			}
		};
	public:
		void merge(inlist<T, NODE>& rhs)
		{
			this->merge(rhs, Less());
		}
		void splice(iterator where, inlist<T, NODE>& right)
		{
			this->splice(where, right, right.begin(), right.end());
		}
		void splice(iterator where, inlist<T, NODE>& right, iterator first)
		{
			this->splice(where, right, first, right.end());
		}
		void splice(iterator where, inlist<T, NODE>& right, iterator first, iterator stop)
		{
			for (iterator it = first; it != stop; )
			{
				T* item = it.m_t;
				it = right.erase(it);
				this->insert(where, item);
			}
		}
		void swap(inlist<T, NODE>& right)
		{
			const u64 thisSize = this->size();
			const u64 rightSize = right.size();
			for (u64 i = 0; i < thisSize; ++i)
			{
				iterator it = this->begin();
				T* item = it.m_t;
				it = this->erase(it);
				right.push_back(item);
			}
			for (u64 i = 0; i < rightSize; ++i)
			{
				iterator it = right.begin();
				T* item = it.m_t;
				it = right.erase(it);
				this->push_back(item);
			}
		}
	private:
		//Declare as non-copy-able
		inlist(const inlist<T, NODE>&);
		inlist<T, NODE>& operator=(const inlist<T, NODE>&);
		static T*& prev(T* t)
		{
			return (t->*NODE).m_prev;
		}
		static T*& next(T* t)
		{
			return (t->*NODE).m_next;
		}
		T* m_head;
		T* m_tail;
		u64 m_size;
	};
	class atDNodeEmptyClass
	{
	};
	template <typename _Type, typename _Base = atDNodeEmptyClass>
	class atDNode : public _Base
	{
	public:
		atDNode<_Type, _Base>()
		{
			Next = NULL; Prev = NULL;
		}
		~atDNode<_Type, _Base>()
		{}
		atDNode<_Type, _Base>(const _Type& d)
		{
			Data = d; Next = NULL; Prev = NULL;
		}
		const atDNode<_Type, _Base>* GetNext() const
		{
			return Next;
		}
		atDNode<_Type, _Base>* GetNext()
		{
			return Next;
		}
		void SetNext(atDNode<_Type, _Base>* next)
		{
			Next = next;
		}
		const atDNode<_Type, _Base>* GetPrev() const
		{
			return Prev;
		}
		atDNode<_Type, _Base>* GetPrev()
		{
			return Prev;
		}
		void SetPrev(atDNode<_Type, _Base>* prev)
		{
			Prev = prev;
		}
		void Insert(atDNode<_Type, _Base>& node)
		{
			node.SetNext(this);
			node.SetPrev(Prev);
			if (Prev)
			{
				Prev->SetNext(&node);
			}
			Prev = &node;
		}
		void Append(atDNode<_Type, _Base>& node)
		{
			node.SetNext(Next);
			node.SetPrev(this);
			if (Next)
			{
				Next->SetPrev(&node);
			}
			Next = &node;
		}
		void Detach()
		{
			if (Next)
			{
				Next->SetPrev(Prev);
			}
			if (Prev)
			{
				Prev->SetNext(Next);
			}
			Next = NULL;
			Prev = NULL;
		}

	public:
		// data stored at this node
		_Type Data;

	protected:
		// next node in list
		atDNode<_Type, _Base>* Next;
		// previous node in list
		atDNode<_Type, _Base>* Prev;
	};
	template <typename _Type, typename _Base = atDNodeEmptyClass>
	class atDList
	{
	public:

		// PURPOSE: Constructor
		atDList<_Type, _Base>()
		{
			Head = NULL; Tail = NULL;
		}
		void Empty()
		{
			Head = NULL; Tail = NULL;
		}
		void RemoveAll()
		{
			for (atDNode<_Type, _Base>* pNext, *pNode = GetHead(); pNode; pNode = pNext)
			{
				pNext = pNode->GetNext();
				pNode->SetNext(NULL);
				pNode->SetPrev(NULL);
			}

			Empty();
		}
		void DeleteAll()
		{
			while (Tail)
			{
				atDNode<_Type, _Base>* node = PopTail();
				delete node;
			}
		}
		const atDNode<_Type, _Base>* GetHead() const
		{
			return Head;
		}
		atDNode<_Type, _Base>* GetHead()
		{
			return Head;
		}
		const atDNode<_Type, _Base>* GetTail() const
		{
			return Tail;
		}
		atDNode<_Type, _Base>* GetTail()
		{
			return Tail;
		}
		void Prepend(atDNode<_Type, _Base>& other)
		{
			if (!other.Head)
				return; // Nothing to do

			if (Head)
			{
				other.Tail->SetNext(Head); // Link other's tail to head
				Head->SetPrev(other.Tail);
				Head = other.Head; // Set new tail pointer
			}
			else
			{
				*this = other; // Copy
			}

			other.Empty(); // Reset other list
		}
		void Append(atDNode<_Type, _Base>& other)
		{
			if (!other.Head)
				return; // Nothing to do

			if (Head)
			{
				other.Head->SetPrev(Tail); // Link other's tail to head
				Tail->SetNext(other.Head);
				Tail = other.Tail; // Set new tail pointer
			}
			else
			{
				*this = other; // Copy
			}

			other.Empty(); // Reset other list
		}
		void Prepend(atDList<_Type, _Base>& node)
		{
			if (Head)
			{
				Head->Insert(node);
				Head = &node;
			}
			else
			{
				Head = &node;
				Tail = &node;
			}
		}
		void Append(atDList<_Type, _Base>& node)
		{
			if (Tail)
			{
				Tail->Append(node);
				Tail = &node;
			}
			else
			{
				Head = &node;
				Tail = &node;
			}
		}
		void InsertAfter(atDNode<_Type, _Base>& insertPoint, atDNode<_Type, _Base>& node)
		{
			insertPoint.Append(node);

			if (&insertPoint == Tail)
			{
				Tail = &node;
			}
		}
		void InsertBefore(atDNode<_Type, _Base>& insertPoint, atDNode<_Type, _Base>& node)
		{
			insertPoint.Insert(node);

			if (&insertPoint == Head)
			{
				Head = &node;
			}
		}
		atDNode<_Type, _Base>* PopHead()
		{
			atDNode<_Type, _Base>* oldHead = Head;

			if (Head)
			{
				Head = Head->GetNext();

				if (Head == NULL)
				{
					Tail = NULL;
				}

				oldHead->Detach();
			}

			return oldHead;
		}
		atDNode<_Type, _Base>* PopTail()
		{
			atDNode<_Type, _Base>* oldTail = Tail;

			if (Tail)
			{
				Tail = Tail->GetPrev();

				if (Tail == NULL)
				{
					Head = NULL;
				}

				oldTail->Detach();
			}

			return oldTail;
		}
		void PopNode(atDNode<_Type, _Base>& node)
		{
			if (&node == Head)
			{
				Head = node.GetNext();
			}
			if (&node == Tail)
			{
				Tail = node.GetPrev();
			}
			node.Detach();
		}
	private:
		// beginning of the list
		atDNode<_Type, _Base>* Head;
		// end of the list
		atDNode<_Type, _Base>* Tail;
	};
}
//pgXXXXXXX
namespace rage {
	// "borrowed" from rage
	struct pgBaseNode
	{
		pgBaseNode* Next;
	};
	class pgBase
	{
	public:
		virtual ~pgBase() = default;
		pgBaseNode* m_FirstNode;
	};
	class pgDictionaryBase : pgBase
	{
	public:
	};
	#define datPadding64(count,name)	datPadding<count> name;
	template <typename T>
	class pgDictionary : public pgDictionaryBase
	{
	public:
		T* Lookup(u32 code) const
		{
			const pgDictionary* current = this;
			do
			{
				int idx = current->m_Codes.binary_search(code);
				if (idx != -1)
					return current->m_Entries[idx];
				current = current->m_Parent;
			} while (current);
			return NULL;
		}
		T* LookupLocal(u32 code) const
		{
			int idx = m_Codes.binary_search(code);
			if (idx != -1)
				return m_Entries[idx];
			else
				return NULL;
		}
		T* LookupLocal(const char* name) const
		{
			return LookupLocal(atStringHash(name));
		}
		typedef bool (*DictionaryCB)(T&, u32, void*);
		bool ForAll(DictionaryCB cb, void* data)
		{
			bool result = true;
			for (int i = 0; i < m_Entries.size(); ++i)
				if ((result = cb(*m_Entries[i], m_Codes[i], data)) == false)
					break;
			return result;
		}
		bool AddEntry(u32 code, T* data)
		{
			for (int i = 0; i < m_Codes.size(); ++i)
			{
				if (m_Codes[i] == code)
					break;
				else if (m_Codes[i] > code)
				{
					m_Codes.push_back(i);
					m_Codes[i] = code;
					m_Entries.push_back(i);
					m_Entries[i] = data;
					return true;
				}
			}
			return false;
		}
		bool AddEntry(const char* name, T* data)
		{
			u32 code = atStringHash(name);
			return AddEntry(code, data);
		}
		int GetRefCount() const
		{
			return m_RefCount;
		}
		int GetCount() const
		{
			return m_Codes.GetCount();
		}
		u32 GetCode(u32 idx) const
		{
			return m_Codes[idx];
		}
		T* GetEntry(u32 idx) const
		{
			return m_Entries[idx];
		}
		void SetEntryUnsafe(u32 idx, T* entry)
		{
			m_Entries[idx] = entry;
		}
		pgDictionary<T>* m_Parent;
		int m_RefCount;
		datPadding64(4, m_Padding);
		atArray<u32> m_Codes;
		atArray<datOwner<T>> m_Entries;
	};
	class pgStreamer
	{
	public:
		typedef u32 Handle;
		typedef void* ReadId;
	};
}
//sysXXXXXXX
namespace rage
{
	namespace sysObfuscatedTypes
	{
		extern u32 obfRand();
	}
	#define sysMemSet memset
	// "borrowed" from rage
	struct sysMemContainerData
	{
		void* m_Base;
		uint32_t m_Size;
	};
	typedef struct sysIpcSemaTag* sysIpcSema;
	class sysMemAllocator
	{
	public:
		virtual ~sysMemAllocator() = default;
	};
	template <typename T, bool mutate = true>
	class sysObfuscated
	{
	public:
		void Init()
		{
			m_xor = sysObfuscatedTypes::obfRand();
			if (mutate)
			{
				m_mutate = sysObfuscatedTypes::obfRand();
			}
		}
		T Get()
		{
			u32 xorVal = m_xor ^ (u32)(size_t)this;
			u32 ret[sizeof(T) / sizeof(u32)];
			u32* src = const_cast<u32*>(&m_data[0]);
			u32* dest = (u32*)&ret;
			for (size_t i = 0; i < sizeof(T) / 4; ++i)
			{
				if (mutate)
				{
					// Extract valid data from two words of storage
					u32 a = *src & m_mutate;
					u32 b = src[sizeof(T) / 4] & (~m_mutate);
					// Apply entropy in the unused bits: Just flip the two u16's in the u32. We can't do a
					// huge amount more without knowledge of the mutation mask.
					u32 entropyA = ((*src & (~m_mutate)) << 16) | ((*src & (~m_mutate)) >> 16);
					u32 entropyB = ((src[sizeof(T) / 4] & m_mutate) << 16) | ((src[sizeof(T) / 4] & m_mutate) >> 16);
					*src = (*src & m_mutate) | entropyA;
					src[sizeof(T) / 4] = (src[sizeof(T) / 4] & (~m_mutate)) | entropyB;

					*dest++ = a | b;
					++src;
				}
				else
				{
					*dest++ = *src++ ^ xorVal;
				}
			}
			// Call Set() to reset the xor and mutate keys on every call to Get()
			if (mutate)
			{
				const_cast<sysObfuscated<T, mutate>*>(this)->Set(*(T*)&ret);
			}
			return *(T*)&ret;
		}
		void Set(T data)
		{
			Init();
			u32 xorVal = m_xor ^ (u32)(size_t)this;
			u32* src = (u32*)&data;
			u32* dest = &m_data[0];
			for (size_t i = 0; i < sizeof(T) / 4; ++i)
			{
				if (mutate)
				{
					u32 a = *src & m_mutate;
					u32 b = *src & (~m_mutate);
					++src;
					*dest = a;
					dest[sizeof(T) / 4] = b;
					++dest;
				}
				else
				{
					*dest++ = *src++ ^ xorVal;
				}
			}
		}
		void operator=(T data)
		{
			Set(data);
		}
		operator T()
		{
			return Get();
		}
		bool operator==(sysObfuscated<T> data)
		{
			return Get() == data.Get();
		}
		bool operator!=(sysObfuscated<T> data)
		{
			return Get() != data.get();
		}
		bool operator==(T data)
		{
			return Get() == data;
		}
		bool operator!=(T data)
		{
			return Get() != data;
		}
	private:
		u32 m_data[(mutate ? sizeof(T) * 2 : sizeof(T)) / sizeof(u32)];
		// XOR and mutate keys for this type
		u32 m_xor;
		u32 m_mutate;
	};
}
struct CellGcmTexture
{
	uint8_t format;
	uint8_t mipmap;
	uint8_t dimension;
	uint8_t cubemap;
	uint32_t remap;
	uint16_t width;
	uint16_t height;
	uint16_t depth;
	uint8_t location;
	uint8_t _padding;
	uint32_t pitch;
	uint32_t offset;
};
//grcXXV
namespace rage {
	namespace grcRSV
	{	// RenderStateValue
		enum
		{
			// FILLMODE:
			FILL_POINT = 0x01, FILL_WIREFRAME = 0x2, FILL_SOLID = 0x03,
			// ZWRITEENABLE is a bool
			// ALPHATESTENABLE is a bool
			// SRCBLEND / DESTBLEND:
			BLEND_ZERO = 1, BLEND_ONE = 2, BLEND_SRCCOLOR = 3, BLEND_INVSRCCOLOR = 4,
			BLEND_SRCALPHA = 5, BLEND_INVSRCALPHA = 6, BLEND_DESTALPHA = 7, BLEND_INVDESTALPHA = 8,
			BLEND_DESTCOLOR = 9, BLEND_INVDESTCOLOR = 10,
			BLEND_SRCALPHASAT = 11, BLEND_BLENDFACTOR = 14, BLEND_INVBLENDFACTOR = 15,
			// These are DX10+ only:
			BLEND_SRC1COLOR = 16, BLEND_INVSRC1COLOR = 17, BLEND_SRC1ALPHA = 18, BLEND_INVSRC1ALPHA = 19,
			// CULLMODE:
			CULL_NONE = 1, CULL_CW = 3, CULL_CCW = 2,
			CULL_FRONT = CULL_CCW, CULL_BACK = CULL_CW,
			// ZFUNC:
			CMP_NEVER = 1, CMP_LESS = 2, CMP_EQUAL = 3, CMP_LESSEQUAL = 4, CMP_GREATER = 5, CMP_NOTEQUAL = 6, CMP_GREATEREQUAL = 7, CMP_ALWAYS = 8,
			// ALPHAREF is 0-255
			// ALPHAFUNC is the same as ZFUNC
			// ALPHABLENDENABLE is a bool
			// STENCILENABLE is a bool
			// STENCILFAIL, STENCILZFAIL, STENCILPASS:
			STENCILOP_KEEP = 1, STENCILOP_ZERO = 2, STENCILOP_REPLACE = 3, STENCILOP_INCRSAT = 4,
			STENCILOP_DECRSAT = 5, STENCILOP_INVERT = 6, STENCILOP_INCRWRAP = 7, STENCILOP_DECRWRAP = 8,
			// STENCILFUNC is the same as ZFUNC
			// Well, not on PSP2.  It's the CMP_ functions shifted left 25.
			// STENCILREF is 0-255
			// STENCILMASK is a u32
			// STENCILWRITEMASK is a u32
			// COLORWRITEENABLE: (bitmask of the following)
			COLORWRITEENABLE_NONE = 0,
			COLORWRITEENABLE_RED = 1, COLORWRITEENABLE_GREEN = 2, COLORWRITEENABLE_BLUE = 4, COLORWRITEENABLE_ALPHA = 8,
			COLORWRITEENABLE_RGB = 7, COLORWRITEENABLE_ALL = 15,
			// BLENDOP:
			BLENDOP_ADD = 1, BLENDOP_SUBTRACT = 2, BLENDOP_REVSUBTRACT = 3, BLENDOP_MIN = 4, BLENDOP_MAX = 5,
			// ALPHATOMASKOFFSETS
			ALPHATOMASKOFFSETS_DITHERED = 0x00000087, ALPHATOMASKOFFSETS_SOLID = 0x000000AA,
		};
	}
	namespace grcSSV
	{ // SamplerStateValue
		enum
		{
			// ADDRESSU/V/W:
			TADDRESS_WRAP = 1, TADDRESS_MIRROR = 2, TADDRESS_CLAMP = 3, TADDRESS_BORDER = 4, TADDRESS_MIRRORONCE = 5,
			// BORDERCOLOR is a 32bit color
			// MAGFILTER/MINFILTER/MIPFILTER
			TEXF_NONE = 0, TEXF_POINT = 1, TEXF_LINEAR = 2, TEXF_ANISOTROPIC = 3,
			TEXF_QUINCUNX = 4, TEXF_GAUSSIAN = 5, TEXF_QUINCUNX_ALT = 6,	// PS3-specific
			TEXF_PYRAMIDALQUAD = 6, TEXF_GAUSSIANQUAD = 7,	// PC-specific
			// MIPMAPLODBIAS is a float
			// MAXMIPLEVEL is a u32
			// MAXANISOTROPY is a u32
			// On PS3 it's a slope value from a table:
			// 1, 1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5,
			// 3.75, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 9, 10, 11, 12, 13, 14, and 15.
			// ONESIXTH implies 2/3 left over, slope = 3/2 = 1.5 (index 4)
			// ONEFOURTH implies 1/2 left over, slope = 2 (index 8)
			// THREEEIGHTHS implies 1/4 left over, slope = 4 (index 16)
			TRILINEAR_IMMEDIATE = 0, TRILINEAR_ONESIXTH = 4, TRILINEAR_ONEFOURTH = 8, TRILINEAR_THREEEIGHTHS = 16,
			// PS3-specific:
			TEXTUREZFUNC_NEVER = (0), TEXTUREZFUNC_LESS = (1), TEXTUREZFUNC_EQUAL = (2), TEXTUREZFUNC_LEQUAL = (3),
			TEXTUREZFUNC_GREATER = (4), TEXTUREZFUNC_NOTEQUAL = (5), TEXTUREZFUNC_GEQUAL = (6), TEXTUREZFUNC_ALWAYS = (7),
			// DX10-specific for now.  (Other platforms will remap via LUT)
			// Example: MIN_LINEAR_MAG_MIP_POINT means "minification is linear, magnification and mip are point (nearest).
			FILTER_MIN_MAG_MIP_POINT = 0,
			FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
			FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
			FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
			FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
			FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
			FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
			FILTER_MIN_MAG_MIP_LINEAR = 0x15,
			FILTER_ANISOTROPIC = 0x55,
			FILTER_GAUSSIAN = 0x55,			// Same as anisotropic on other platforms
			FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
			FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
			FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
			FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
			FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
			FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
			FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
			FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
			FILTER_COMPARISON_ANISOTROPIC = 0xd5,
		};
	}
}
//grcXXXXXXX
namespace rage {
	namespace grcStateBlock
	{
		enum eStateBlockDirty : u8
		{
			DEPTH_STENCIL_STATE_DIRTY = (1 << 0),
			RASTERIZER_STATE_DIRTY = (1 << 1),
			BLEND_STATE_DIRTY = (1 << 2)
		};
	#define INVALID_STATEBLOCK 0
		enum grcDepthStencilStateHandleEnum
		{
			DSS_Invalid = INVALID_STATEBLOCK
		};
		enum grcRasterizerStateHandleEnum
		{
			RS_Invalid = INVALID_STATEBLOCK
		};
		enum grcBlendStateHandleEnum
		{
			BS_Invalid = INVALID_STATEBLOCK
		};
		enum grcSamplerStateHandleEnum
		{
			SS_Invalid = INVALID_STATEBLOCK
		};
		typedef grcStateBlock::grcDepthStencilStateHandleEnum grcDepthStencilStateHandle;
		typedef grcStateBlock::grcRasterizerStateHandleEnum grcRasterizerStateHandle;
		typedef grcStateBlock::grcBlendStateHandleEnum grcBlendStateHandle;
		typedef grcStateBlock::grcSamplerStateHandleEnum grcSamplerStateHandle;
	#define MAX_RAGE_RENDERTARGET_COUNT	8
		struct grcDepthStencilStateDesc
		{
			grcDepthStencilStateDesc() :
				DepthEnable(true),
				DepthWriteMask(true),
				DepthFunc(grcRSV::CMP_LESS),
				StencilEnable(false),
				StencilReadMask(0xFF),
				StencilWriteMask(0xFF)
			{}
			struct grcStencilOpDesc
			{
				grcStencilOpDesc() :
					StencilFailOp(grcRSV::STENCILOP_KEEP),
					StencilDepthFailOp(grcRSV::STENCILOP_KEEP),
					StencilPassOp(grcRSV::STENCILOP_KEEP),
					StencilFunc(grcRSV::CMP_ALWAYS)
				{}
				int StencilFailOp, StencilDepthFailOp, StencilPassOp, StencilFunc;
			};
			int DepthEnable;
			int DepthWriteMask;
			int DepthFunc;
			int StencilEnable;
			u8 StencilReadMask;
			u8 StencilWriteMask;
			grcStencilOpDesc FrontFace, BackFace;
		};
		struct grcRasterizerStateDesc
		{
			grcRasterizerStateDesc() :
				FillMode(grcRSV::FILL_SOLID)
				, CullMode(grcRSV::CULL_BACK)
				, FrontCounterClockwise(true)
				, DepthBiasDX10(0)
				, DepthBiasClamp(0.f)
				, SlopeScaledDepthBias(0.f)
				, DepthClipEnable(true)
				, ScissorEnable(true)
				, MultisampleEnable(false)	// would only affect lines under DX10.1+ anyway
				, AntialiasedLineEnable(false)
				, HalfPixelOffset(false),
				DepthBiasDX9(0.f)
			{}

			int FillMode;
			int CullMode;
			int FrontCounterClockwise;
			int DepthBiasDX10; // (this is converted into a float on DX9-era hardware)
			float DepthBiasClamp; // (this has no equivalent on DX9-era hardware)
			float SlopeScaledDepthBias;
			int DepthClipEnable;
			int ScissorEnable;
			int MultisampleEnable;
			int AntialiasedLineEnable;
			int HalfPixelOffset; // DX9 Extension
			float DepthBiasDX9; // DX9 Extension
		};
		struct grcBlendStateDesc
		{
			grcBlendStateDesc() :
				AlphaToCoverageEnable(false)
				, IndependentBlendEnable(false)
				, AlphaToMaskOffsets(grcRSV::ALPHATOMASKOFFSETS_SOLID)
			{}
			struct grcRenderTargetBlendDesc
			{
				grcRenderTargetBlendDesc() :
					BlendEnable(false),
					SrcBlend(grcRSV::BLEND_ONE),
					DestBlend(grcRSV::BLEND_ZERO),
					BlendOp(grcRSV::BLENDOP_ADD),
					SrcBlendAlpha(grcRSV::BLEND_ONE),
					DestBlendAlpha(grcRSV::BLEND_ZERO),
					BlendOpAlpha(grcRSV::BLENDOP_ADD),
					RenderTargetWriteMask(grcRSV::COLORWRITEENABLE_ALL)
				{}
				int BlendEnable;
				int SrcBlend;
				int DestBlend;
				int BlendOp;
				int SrcBlendAlpha;
				int DestBlendAlpha;
				int BlendOpAlpha;
				u8 RenderTargetWriteMask;
			};
			int AlphaToCoverageEnable;
			int IndependentBlendEnable;
			grcRenderTargetBlendDesc BlendRTDesc[MAX_RAGE_RENDERTARGET_COUNT];
			int AlphaToMaskOffsets; // 360 extension
		};
		struct grcSamplerStateDesc
		{
			grcSamplerStateDesc() :
				Filter(grcSSV::FILTER_MIN_MAG_MIP_LINEAR)
				, AddressU(grcSSV::TADDRESS_WRAP)
				, AddressV(grcSSV::TADDRESS_WRAP)
				, AddressW(grcSSV::TADDRESS_WRAP)
				, MipLodBias(0.0f)
				, MaxAnisotropy(1)
				, ComparisonFunc(grcRSV::CMP_NEVER)
				, BorderColorRed(0), BorderColorGreen(0), BorderColorBlue(0), BorderColorAlpha(0)
				, MinLod(0.0f)
				, MaxLod(12.0f)
				, TrilinearThresh(grcSSV::TRILINEAR_ONEFOURTH)
				, AlphaKill(false)
			{}
			int Filter; // FILTER_... type (new to D3D10; subsumes old Min/Mag/Mip states.)
			int AddressU, AddressV, AddressW;
			float MipLodBias;
			u32 MaxAnisotropy;
			int ComparisonFunc; // (only used by certain filter modes, DX10 only)
			float BorderColorRed, BorderColorGreen, BorderColorBlue, BorderColorAlpha;
			float MinLod;
			float MaxLod;
			int TrilinearThresh; // 360/PS3 extension: trilinear threshold.
			int AlphaKill; // PS3 extension: alpha kill functionality, see cellGcmSetTextureControlAlphaKill docs
		};
		struct grcDepthStencilStateDesc;
		struct grcRasterizerStateDesc;
		struct grcBlendStateDesc;
		struct grcDepthStencilState
		{
			u8 DepthEnable, DepthWriteMask;
			u8 StencilEnable;
			u8 TwoSidedStencil;
			u16 DepthFunc; // CMP_... in effect_values.h; default CMP_LESS
			u8 StencilReadMask; // default: 0xFF
			u8 StencilWriteMask; // default: 0xFF
			u16 FrontStencilFailOp, FrontStencilDepthFailOp, FrontStencilPassOp, FrontStencilFunc;
			u16 BackStencilFailOp, BackStencilDepthFailOp, BackStencilPassOp, BackStencilFunc;
		};
		struct grcRasterizerState
		{
			u16 FillMode;
			u16 CullMode;
			union
			{
				float f; u32 u;
			} DepthBias;
			float DepthBiasClamp;
			union
			{
				float f; u32 u;
			} SlopeScaledDepthBias;
			// u8 DepthClipEnable;
			// u8 FrontCounterClockwise;
			// u16 ScissorEnable;
			u32 MultisampleEnable : 1, HalfPixelOffset : 1, pad : 30;	// Make sure any pad bits are initialized or duplicate state merging will fail!
		};
		struct grcBlendState
		{
			struct Target
			{
				u8 BlendEnable;
				u8 RenderTargetWriteMask;
				u16 SrcBlend, DestBlend, BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha;
			};
			u8 AlphaToCoverageEnable, IndependentBlendEnable;
			u16 AlphaTestComparison;
			int AlphaTestEnable;	// this could be a u8 but we pad it out for alignment.
			Target Targets[8];
		};
		struct grcSamplerState
		{
			u8 MinFilter, MagFilter, MipFilter;	// Translated from DX11 enumerated types
			u8 AddressU, AddressV, AddressW; // TADDRESS_CLAMP
			u8 MaxAnisotropy; // 16
			u8 BorderColor, BorderColorW; // only black(0) or white(~0) are supported on DX9-era hardware.  (Xenon supports "opaque" black too)
			u8 TrilinearThresh;
			u8 MinMipLevel, MaxMipLevel; // MinMipLevel == MaxLod, MaxMipLevel == MinLod
			union
			{
				u32 u; float f;
			} MipLodBias; // 0.f
			int CompareFunc;
		};
		template <typename H, typename T, typename D, typename S, typename indexType, u32 maxSize>
		class grcStateBlockStore
		{
			static constexpr u16 MaxRefCount = 65535;
		public:
			grcStateBlockStore()
			{
				FirstUsed = maxSize;
				FirstFree = 0;
				Used = MaxUsed = 0;
				for (u32 i = 0; i < maxSize; i++)
				{
					RefCounts[i] = 0;
					Next[i] = indexType(i + 1);
					State[i] = NULL;
				}
			}
			const T& operator[](u64 i)
			{
				i--;
				return Store[i];
			}
			H Allocate(const T& data, const D& desc)
			{
				u32 hash = atDataHash((char*)&data, sizeof(data)), i;
				for (i = FirstUsed; i != maxSize && (Hashes[i] != hash || memcmp(&data, Store + i, sizeof(data))); i = Next[i])
					;
				if (i == maxSize)
				{
					if (FirstFree == maxSize)
					{
						return H();
					}
					else
					{
						// Pull head entry off the free list, add it to the used list
						i = FirstFree;
						FirstFree = Next[i];
						Next[i] = FirstUsed;
						FirstUsed = indexType(i);

						// Remember the new hash and refcount
						Hashes[i] = hash;
						RefCounts[i] = 1;
						Store[i] = data;
						//State[i] = T::AllocateState(desc);
						// Track highest entry ever used (so we know how many to transfer down to SPU)
						if (i + 1 > MaxUsed)
							MaxUsed = indexType(i + 1);
					}
				}
				else if (RefCounts[i] != MaxRefCount)
					RefCounts[i]++;
				return static_cast<H>(i + 1);
			}
			void RecreateState(H i, const D& desc)
			{
				i = static_cast<H>(i - 1);
				State[i]->Release();
				//State[i] = T::AllocateState(desc);
			}
			S* GetState(H i)
			{
				i = static_cast<H>(i - 1);
				return State[i];
			}
			void AddRef(H i)
			{
				i = static_cast<H>(i - 1);
				if (RefCounts[i] != MaxRefCount)
					++RefCounts[i];
			}
			void Release(H i)
			{
				i = static_cast<H>(i - 1);
				if (RefCounts[i] != MaxRefCount && !--RefCounts[i])
				{
					State[i]->Release();
					State[i] = NULL;
					// Find ourselves on the used list, remove self
					indexType* prev = &FirstUsed;
					while (*prev != i)
						prev = &Next[*prev];	// will crash here if you passed in an invalid or previously freed handle
					*prev = Next[i];
					// Reinsert ourself onto the head of the free list.
					Next[i] = FirstFree;
					FirstFree = indexType(i);
					--Used;
				}
			}
			T* GetStore()
			{
				return (T*)((char*)Store - (sizeof(T)));
			}	// NOTE THE "-1" EQUIVALENT HERE, avoids -1 on every handle lookup on SPU
			S** GetStates()
			{
				return State - 1;
			} // NOTE THE -1 HERE, avoids -1 on every handle lookup on SPU
			const indexType* GetFreeList() const
			{
				return Next;
			}
			indexType GetFirstUsed() const
			{
				return FirstUsed;
			}
			u32 GetMaxUsed() const
			{
				return MaxUsed;
			}
			u32 GetMaxSize() const
			{
				return maxSize;
			}
		private:
			T Store[maxSize]; // the store itself
			u32 Hashes[maxSize]; // hash codes to speed up comparisons against existing types
			u16 RefCounts[maxSize]; // reference counts
			S* State[maxSize];
			indexType Next[maxSize]; // next item in list (every object is either on free list or used list)
			indexType FirstUsed, FirstFree; // head pointers for each list
			indexType Used, MaxUsed;
		};
	}
	enum grcDrawMode
	{
		drawPoints,			// Draw one or more single-pixel points
		drawLines,			// Draw one or more disconnected line segments
		drawLineStrip,		// Draw a single multivertex line strip
		drawTris,			// Draw one or more disconnected triangles
		drawTriStrip,		// Draw a single tristrip
		drawTriFan,			// Draw a single triangle fan (not supported on DX11
		drawQuads,			// Independent quads; ONLY SUPPORTED UNDER Xenon, PS3, and Orbis
		drawRects,			// Independent rectangles; ONLY SUPPORTED UNDER Xenon and Orbis.  Three vertices per rectangle: x1,y1,z,u1,v1 / x2,y1,z,u2,v1 / x1,y2,z,u1,v2
		drawTrisAdj,		// Triangles with adjacency information
		drawModesTotal,
	};
	struct grcCellGcmTextureWrapper : CellGcmTexture
	{
		inline void	SetWidth(uint16_t in)
		{
			width = in;
		}
		inline uint16_t GetWidth() const
		{
			return width;
		};
		inline void	SetHeight(uint16_t in)
		{
			height = in;
		}
		inline uint16_t GetHeight() const
		{
			return height;
		};
		inline void	SetDepth(uint16_t in)
		{
			depth = in;
		}
		inline uint16_t GetDepth() const
		{
			return depth;
		};
		inline void	SetFormat(uint8_t in)
		{
			format = in;
		}
		inline uint8_t GetFormat() const
		{
			return format;
		};
		inline void	SetDimension(uint8_t in)
		{
			dimension = in;
		}
		inline uint8_t GetDimension() const
		{
			return dimension;
		};
		inline void	SetMipMap(uint8_t in)
		{
			mipmap = in;
		}
		inline uint8_t GetMipMap() const
		{
			return mipmap;
		};
		inline void	SetBindFlag(uint8_t in)
		{
			_padding = in;
		}
		inline uint8_t GetBindFlag() const
		{
			return _padding;
		};
		inline void	SetTileMode(uint8_t in)
		{
			location = in;
		}
		inline uint8_t GetTileMode() const
		{
			return location;
		};
		inline void	SetImageType(uint8_t in)
		{
			cubemap = in;
		}
		inline uint8_t GetImageType() const
		{
			return cubemap;
		};
		inline void	SetOwnsMem(uint32_t in)
		{
			remap = in;
		}
		inline uint32_t GetOwnsMem() const
		{
			return remap;
		};
		inline void	SetUsesPreAllocatedMem(uint32_t in)
		{
			pitch = in;
		}
		inline uint32_t GetUsesPreAllocatedMem() const
		{
			return pitch;
		};
	};
	typedef struct ID3D11Device grcDeviceHandle;
	typedef struct IDXGISwapChain grcSwapChain;
	typedef struct ID3D11DeviceContext grcContextHandle;
	typedef struct ID3D11CommandList grcContextCommandList;
	typedef ID3D11Buffer grcBuffer;
	typedef ID3D11ComputeShader grcComputeShader;
	typedef ID3D11DeviceChild grcShader;
	typedef ID3D11DomainShader grcDomainShader;
	typedef ID3D11GeometryShader grcGeometryShader;
	typedef ID3D11HullShader grcHullShader;
	typedef ID3D11PixelShader grcPixelShader;
	typedef ID3D11Resource grcDeviceTexture;
	typedef ID3D11Resource grcTextureObject;
	typedef ID3D11ShaderResourceView grcDeviceView;
	typedef ID3D11VertexShader grcVertexShader;
	typedef IUnknown grcDeviceSurface;
	typedef IUnknown grcResource;
	typedef IUnknown grcBlob;
	typedef void grcStreamOutputDeclaration;
	class grcDevice {
	public:
		enum MSAAModeEnum {
			MSAA_None = 0,
			MSAA_NonMaskAble = 1,
			MSAA_2 = 2,
			MSAA_4 = 4,
			MSAA_8 = 8,
			// PS3
			MSAA_2xMS = 2, // using 2x multi-sampling with a Quincunx filter while downsampling
			MSAA_Centered4xMS = 4, // using 4x multi-sampling with a centered filter kernel and a Gauss filter while downsampling
			MSAA_Rotated4xMS = 8 // using 4x multi-sampling with a rotated filter kernel and a Gauss filter while downsampling
		};
		enum Stereo_t { MONO, STEREO, AUTO };
		class MSAAMode {
		public:
			inline MSAAMode()
				: m_Mode(MSAA_None) {}
			inline MSAAMode(MSAAModeEnum mode)
				: m_Mode(mode) {}
			inline MSAAMode(int mode)
				: m_Mode(static_cast<MSAAModeEnum>(mode)) {}
			inline MSAAMode operator=(int value) {
				m_Mode = static_cast<MSAAModeEnum>(value);
				return *this;
			}
			inline bool operator==(MSAAMode mode) const {
				return m_Mode == mode.m_Mode;
			}
			inline bool operator==(MSAAModeEnum mode) const {
				return m_Mode == mode;
			}
			inline operator u32() const {
				return static_cast<u32>(m_Mode);
			}
		private:
			MSAAModeEnum m_Mode;
		};
	};
	enum grcLockType {
		grcsRead = 0x01,
		grcsWrite = 0x02,
		grcsDoNotWait = 0x04,	// Will return a failed lock if unsuccessful
		grcsDiscard = 0x08,	// Discard contents of locked region entire buffer
		grcsNoOverwrite = 0x10, // Indicates that the region locked should not be in use by the GPU - Can cause visual artifacts if not used properly
		grcsNoDirty = 0x20,	// Prevents driver from marking the region as dirty
		grcsAllowVRAMLock = 0x40, // don't assert VRAM lock if you know what you're doing
	};
	struct grcTextureLock {
		int MipLevel;
		void* Base;
		int Pitch;
		int BitsPerPixel;
		int Width, Height;
		int RawFormat; // D3DFMT_... on D3D builds, or SCE_GS_PSM_... on PSX2.
		int Layer; // Represents either face of cubemap or layer of volume texture
	};
	struct grcPoint {
		int x, y;
	};
	typedef struct grcRect {
		int x1, y1;
		int x2, y2;
	} GRCRECT;
	struct grcParameterAnnotation {
		u32 NameHash;
		enum AnnoType { AT_INT, AT_FLOAT, AT_STRING } Type;
		union {
			int Int;
			float Float;
			const char* String;
		};
	};
	typedef void grcBufferBasic;
	typedef void grcBufferUAV;
	typedef void grcTextureUAV;
	class grcInstanceData {
	public:
		void Clone(grcInstanceData& outClone) const {
			outClone.Basis = Basis;
			if (Flags & FLAG_MATERIAL)
				outClone.Material = const_cast<grcInstanceData*>(this);
			else
				outClone.Material = NULL;
			outClone.Count = Count;
			outClone.TotalSize = TotalSize;
			outClone.SpuSize = SpuSize;
			outClone.DrawBucket = DrawBucket;
			outClone.DrawBucketMask = DrawBucketMask;
			outClone.PhysMtl_DEPRECATED = PhysMtl_DEPRECATED;
			outClone.pad = pad;
			outClone.TextureCount = TextureCount;
			outClone.IsInstanced = IsInstanced;
			outClone.UserFlags = UserFlags;
			outClone.Flags = Flags & ~FLAG_MATERIAL;
			char *buffer = (TotalSize ? new char[TotalSize] : NULL);
			outClone.Entries = (grcInstanceData::Entry*)buffer;
			memcpy(buffer, Entries, TotalSize);
			ptrdiff_t fixup = buffer - (char*)Entries;
			for (int i = 0; i < TextureCount; i++) {
				outClone.Data()[i].Texture = Data()[i].Texture;
			}
			for (int i = TextureCount; i < Count; i++) {
				outClone.Data()[i].Any = (void*)((char*)Data()[i].Any + fixup);
			}
		}
		struct Entry {
			u8 Count;
			u8 Register;
			u8 SamplerStateSet, SavedSamplerStateSet;
			union {
				float* Float;
				void* Texture;
				const grcBufferUAV* RO_Buffer;
				grcBufferUAV* RW_Buffer;
				const grcTextureUAV* RW_Texture;
				void* Any;
			};
		};
		__forceinline Entry* Data() const { return Entries; }
		Entry* Entries;
		union {
			class grcEffect* Basis;
			u32 BasisHashCode;
		};
		enum { // For 'Flags' immediately below
			FLAG_MATERIAL = 0x01, // This instanceData is really a material
			FLAG_EXTRA_DATA = 0x80, // This instanceData already has its MTR-expanded buffers allocated (if relevant)
		};
		u8 Count, DrawBucket, PhysMtl_DEPRECATED, Flags;
		u16 SpuSize, TotalSize;
		union {
			grcInstanceData* Material;
			u32 MaterialHashCode;
			const char* MaterialName;
		};
		u32 DrawBucketMask;
		bool IsInstanced;
		u8 UserFlags;
		u8 pad, TextureCount;
		u32 SortKey_DEPRECATED;
	};
	using grcString = const char*;
	#define EFFECT_CONSTANT_BUFFER_COUNT 14
	#define EFFECT_UAV_BUFFER_COUNT 4
	#define EFFECT_SAMPLER_COUNT 16
	#define EFFECT_TEXTURE_COUNT 42
	class grcCBuffer {
	public:
		u32	Size;
		u16 Registers[6];
		u32 NameHash;
		grcString Name;
		grcBuffer* m_pBuffer;
		char *m_pAlignedBackingStore;
		char *m_pAllocatedBackingStore;
		u32 m_BufferStride;
		friend class grcProgram;
		friend class grcDevice;
	};
	enum { TEXTURE_REGISTER_BITS = 6, TEXTURE_USAGE_BITS = 7 };
	class grcParameter {
		friend class grcEffect;
		friend class grcInstanceData;
	public:
		struct Annotation {
			u32 NameHash;											// Annotation name
			enum AnnoType { AT_INT, AT_FLOAT, AT_STRING } Type;		// Discriminator for payload
			union {						// Payload (String is allocated on heap)
				int Int;
				float Float;
				const char* String;
			};
		};
		u8 Type;
		u8 Count;
		u8 DataSize;
		u8 AnnotationCount;
		grcString Name;
		grcString Semantic;
		u32 NameHash;
		u32 SemanticHash;
		Annotation* Annotations;
		void* Data;
		u16 Register : TEXTURE_REGISTER_BITS;
		u16 TextureType : 8 - TEXTURE_REGISTER_BITS;
		u16 Usage : TEXTURE_USAGE_BITS;
		u16 ComparisonFilter : 8 - TEXTURE_USAGE_BITS;
		u8 SamplerStateSet;
		u8 SavedSamplerStateSet;
		u32 CBufferOffset;
		u32 CBufferNameHash;
		union {
			grcCBuffer* CBuf;
			const class grcTexture* Tex;
			const grcBufferUAV* RO_Buffer;
			grcBufferUAV* RW_Buffer;
			const grcTextureUAV* RW_Texture;
		};
	};
	enum ShaderType {
		VS_TYPE, PS_TYPE, CS_TYPE, DS_TYPE, GS_TYPE, HS_TYPE, NUM_TYPES,
		NONE_TYPE = NUM_TYPES
	};
	struct d3d11InputElementDesc {
		const char *SemanticName;
		u32 SemanticIndex;
		u32 Format;
		u32 InputSlot;
		u32 AlignedByteOffset;
		u32 InputSlotClass;
		u32 InstanceDataStepRate;
	};
	struct grcVertexDeclaration {
		int Release() const {
			return !(--refCount) ? refCount : (delete this, 0);
		}
		void AddRef() const {
			++refCount;
		}
		static constexpr u32 c_MaxStreams = 4; // Really 16 under DX11...
		int elementCount;
		mutable int refCount;
		u32 Stream0Size;
		//d3d11InputElementDesc desc[0];
	};
	enum {
		USAGE_VERTEXPROGRAM = 1,
		USAGE_FRAGMENTPROGRAM = 2,
		USAGE_GEOMETRYPROGRAM = 4, // DX10
		USAGE_HULLPROGRAM = 8, // DX11
		USAGE_DOMAINPROGRAM = 16, // DX11
		USAGE_COMPUTEPROGRAM = 32, // DX11
		USAGE_ANYPROGRAM = 63,
		USAGE_MATERIAL = 64, // Parameter is per-material, not per-instance
		USAGE_COMPARISON = 128, // Parameter is sampler with comparison filter
	};
	class grcProgram {
		friend class grcEffect;
		friend class grcInstanceData;
	public:
		virtual ~grcProgram() = default;
		virtual u32 GetProgramSize() const = 0;

		grcString m_EntryName;
		atArray<u32> m_Constants;
		void** m_ppDeviceCBuffers;
		u32 m_CBufferFingerprint;
		u8 m_CBufStartSlot, m_CBufEndSlot;
		s8 m_TexStartSlot, m_TexEndSlot;
		int m_numCBuffers;
		grcCBuffer* m_pCBuffers[EFFECT_CONSTANT_BUFFER_COUNT];
		int m_numTexContainers;
		grcParameter* m_pTexContainers[EFFECT_TEXTURE_COUNT + EFFECT_UAV_BUFFER_COUNT];
		u32 m_HashKey;
	};
	inline grcContextHandle* g_grcCurrentContext{};
	class grcVertexProgram : public grcProgram {
		friend class grcEffect;
	public:
		typedef grcPixelShader* ProgramType;
		ProgramType GetProgram() { return Program; }
		ProgramType GetProgram() const { return Program; }
		void* GetProgramData() { return ProgramData; }
		void* GetProgramData() const { return ProgramData; }
		u32 GetProgramSize() const { return ProgramSize; }
		static ShaderType GetShaderType() { return VS_TYPE; }
		struct DeclSetup {
			const grcVertexDeclaration* FetchDecl;
			ID3D11InputLayout* InputLayout;
			DeclSetup* Next;
		};
		ProgramType Program;
		u32 ProgramSize;
		void* ProgramData;
		DeclSetup* FirstDecl;
	};
	class grcFragmentProgram : public grcProgram {
		friend class grcEffect;
	public:
		typedef grcPixelShader* ProgramType;
		ProgramType GetProgram() { return Program; }
		ProgramType GetProgram() const { return Program; }
		u32 GetProgramSize() const { return ProgramSize; }
		static ShaderType GetShaderType() { return PS_TYPE; }
		ProgramType Program;
		u32 ProgramSize;
	};
	class grcComputeProgram : public grcProgram {
		friend class grcEffect;
	public:
		typedef grcComputeShader* ProgramType;
		ProgramType GetProgram() { return Program; }
		ProgramType GetProgram() const { return Program; }
		u32 GetProgramSize() const { return ProgramSize; }
		static ShaderType GetShaderType() { return CS_TYPE; }
	private:
		ProgramType Program;
		u32 ProgramSize;
	};
	class grcDomainProgram : public grcProgram
	{
		friend class grcEffect;
	public:
		typedef grcDomainShader* ProgramType;
		ProgramType GetProgram()
		{
			return Program;
		}
		ProgramType GetProgram() const
		{
			return Program;
		}
		u32 GetProgramSize() const
		{
			return ProgramSize;
		}
		static ShaderType GetShaderType()
		{
			return DS_TYPE;
		};
	private:
		ProgramType Program;
		u32 ProgramSize;
	};
	class grcGeometryProgram : public grcProgram
	{
		friend class grcEffect;
	public:
		struct grcStream
		{
			char mSemanticName[16];
			u8 mSemanticIndex;
			u8 mStartComponent;
			u8 mComponentCount;
			u8 mOutputSlot;
		};
		typedef grcGeometryShader* ProgramType;
		ProgramType GetProgram()
		{
			return Program;
		}
		ProgramType GetProgram() const
		{
			return Program;
		}
		u32 GetProgramSize() const
		{
			return ProgramSize;
		}
		static ShaderType GetShaderType()
		{
			return GS_TYPE;
		};
	private:
		ProgramType Program;
		u32 ProgramSize;
		grcStream* Streams;
		u32 StreamCount;
	};
	class grcHullProgram : public grcProgram
	{
		friend class grcEffect;
	public:
		typedef grcHullShader* ProgramType;
		ProgramType GetProgram()
		{
			return Program;
		}
		ProgramType GetProgram() const
		{
			return Program;
		}
		u32 GetProgramSize() const
		{
			return ProgramSize;
		}
		static ShaderType GetShaderType()
		{
			return HS_TYPE;
		};
	private:
		ProgramType Program;
		u32 ProgramSize;
	};
	typedef enum grcEffectGlobalVar__ { grcegvNONE } grcEffectGlobalVar;
	typedef enum grcEffectVar__ { grcevNONE } grcEffectVar;
	typedef enum grcEffectTechnique__ { grcetNONE } grcEffectTechnique;
	typedef enum grcEffectAnnotation__ { grceaNONE } grcEffectAnnotation;
	struct grcEffect_Technique_Pass
	{
		u8 VertexProgramIndex, FragmentProgramIndex;
		u8 GeometryProgramIndex, ComputeProgramIndex, DomainProgramIndex, HullProgramIndex;
		u8 RasterizerStateHandle, DepthStencilStateHandle, BlendStateHandle, AlphaRef, StencilRef, pad;
	};
	inline static u8 s_StencilRefMask = 0xFF;
	class grcEffect {
	public:
		static constexpr int MAX_TECHNIQUE_GROUPS = 64;
		grcEffectTechnique LookupTechnique(const char* name) {
			return LookupTechniqueByHash(atStringHash(name));
		}
		grcEffectTechnique LookupTechniqueByHash(u32 hash) {
			for (int i=0; i<m_Techniques.size(); i++)
				if (m_Techniques[i].NameHash == hash)
					return (grcEffectTechnique)(i+1);
			return grcetNONE;
		}
		grcEffectTechnique GetTechniqueByIndex(int idx) const { return (grcEffectTechnique)(idx+1); }
		int GetTechniqueCount() const { return m_Techniques.size(); }
		enum eDrawType { 
			RMC_DRAW, 
			RMC_DRAWSKINNED, 
			RMC_DRAW_TESSELLATED, 
			RMC_DRAWSKINNED_TESSELLATED, 
			RMC_DRAW_INSTANCED,
			RMC_DRAWSKINNED_INSTANCED,
			RMC_DRAW_INSTANCED_TESSELLATED,
			RMC_DRAWSKINNED_INSTANCED_TESSELLATED,
			RMC_COUNT 
		};
		void Clone(grcInstanceData& outClone) const {
			m_InstanceData.Clone(outClone);
		}
		struct Technique
		{
			typedef grcEffect_Technique_Pass Pass;
			u32 NameHash;
			grcString Name;
			atArray<Pass> Passes;
		};
		static u8 GetStencilRefMask() { return s_StencilRefMask; }
		static void SetStencilRefMask(u8 mask) { s_StencilRefMask = mask; }
		atArray<Technique> m_Techniques;
		atArray<grcParameter> m_Locals;
		atArray<grcCBuffer*> m_LocalsCBuf;
		atArray<grcVertexProgram> m_VertexPrograms;
		atArray<grcFragmentProgram> m_FragmentPrograms;
		u8 m_TechniqueMap[MAX_TECHNIQUE_GROUPS][RMC_COUNT];
		sysMemContainerData m_Container;
		char m_EffectName[40];
		grcInstanceData m_InstanceData;
		u64 m_EffectTimeStamp;
		grcString m_EffectPath;
		u32 m_NameHash;
		atArray<grcParameterAnnotation> m_Properties;
		u32 m_Dcl;
		u16 m_Ordinal;
		atArray<void*> m_VarInfo;
		atArray<grcComputeProgram> m_ComputePrograms;
		atArray<grcDomainProgram> m_DomainPrograms;
		atArray<grcGeometryProgram>	m_GeometryPrograms;
		atArray<grcHullProgram> m_HullPrograms;
	};
	enum Format {
		UNKNOWN, // Undefined
		DXT1, // DXT1 (aka 'BC1')
		DXT3, // DXT3 (aka 'BC2')
		DXT5, // DXT5 (aka 'BC3', 'DXT5NM', 'RXGB')
		CTX1, // CTX1 (like DXT1 but anchor colors are 8.8 instead of 5.6.5)
		DXT3A, // alpha block of DXT3 (XENON-specific)
		DXT3A_1111, // alpha block of DXT3, split into four 1-bit channels (XENON-specific)
		DXT5A, // alpha block of DXT5 (aka 'BC4', 'ATI1')
		DXN, // DXN (aka 'BC5', 'ATI2', '3Dc', 'RGTC', 'LATC', etc.) // [CLEMENSP]
		BC6, // BC6 (specifically BC6H_UF16)
		BC7, // BC7
		A8R8G8B8, // 32-bit color with alpha, matches Color32 class
		A8B8G8R8, // 32-bit color with alpha, provided for completeness
		A8, // 8-bit alpha-only (color is black)
		L8, // 8-bit luminance (R=G=B=L, alpha is opaque)
		A8L8, // 16-bit alpha + luminance
		A4R4G4B4, // 16-bit color and alpha
		A1R5G5B5, // 16-bit color with 1-bit alpha
		R5G6B5, // 16-bit color
		R3G3B2, // 8-bit color (not supported on consoles)
		A8R3G3B2, // 16-bit color with 8-bit alpha (not supported on consoles)
		A4L4, // 8-bit alpha + luminance (not supported on consoles)
		A2R10G10B10, // 32-bit color with 2-bit alpha
		A2B10G10R10, // 32-bit color with 2-bit alpha
		A16B16G16R16, // 64-bit four channel fixed point (s10e5 per channel -- sign, 5 bit exponent, 10 bit mantissa)
		G16R16, // 32-bit two channel fixed point
		L16, // 16-bit luminance
		A16B16G16R16F, // 64-bit four channel floating point (s10e5 per channel)
		G16R16F, // 32-bit two channel floating point (s10e5 per channel)
		R16F, // 16-bit single channel floating point (s10e5 per channel)
		A32B32G32R32F, // 128-bit four channel floating point (s23e8 per channel)
		G32R32F, // 64-bit two channel floating point (s23e8 per channel)
		R32F, // 32-bit single channel floating point (s23e8 per channel)
		D15S1, // 16-bit depth + stencil (depth is 15-bit fixed point, stencil is 1-bit) (not supported on consoles)
		D24S8, // 32-bit depth + stencil (depth is 24-bit fixed point, stencil is 8-bit)
		D24FS8, // 32-bit depth + stencil (depth is 24-bit s15e8, stencil is 8-bit)
		P4, // 4-bit palettized (not supported on consoles)
		P8, // 8-bit palettized (not supported on consoles)
		A8P8, // 16-bit palettized with 8-bit alpha (not supported on consoles)
		R8, // non-standard R001 format (8 bits per channel)
		R16, // non-standard R001 format (16 bits per channel)
		G8R8, // non-standard RG01 format (8 bits per channel)
		LINA32B32G32R32F_DEPRECATED,
		LINA8R8G8B8_DEPRECATED,
		LIN8_DEPRECATED,
		RGBE,
		LAST_FORMAT = RGBE
	};
	enum { FORMAT_COUNT = LAST_FORMAT + 1 };
	enum FormatFlags { // doesn't hurt to put this in its own enum, and this would allow switching on Format without 'default'
		FORMAT_FLAG_sRGB = 0x80000000,
		FORMAT_FLAG_LINEAR = 0x40000000,
		FORMAT_FLAG_SYSMEM = 0x20000000,
		FORMAT_MASK = 0x1fffffff,
	};
	enum ImageType {
		STANDARD, // 2D image (still supports mipmaps, and texture array layers)
		CUBE,     // Cube map image (+x,-x,+y,-y,+z,-z, stored as a 6-layer 2D image)
		DEPTH,    // Depth map image
		VOLUME,   // Volume texture
		FORCETYPE32 = 0x7FFFFFFF
	};
	// fuck you^2 R*
	struct RGBA {
		RGBA(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}
		RGBA(u8 r, u8 g, u8 b) : RGBA(r, g, b, 255) {}
		RGBA() = default;
		u8 r, g, b, a;
	};
	class Color32 {
	public:
		Color32() {
			Set(255,255,255,255);
		}
		explicit Color32(u32 color) : m_Color(color) {}
		Color32(int r, int g, int b, int a = 255) { Set(r, g, b, a); }
		Color32(u32 r, u32 g, u32 b, u32 a = 255) { Set(r, g, b, a); }
		Color32(float r, float g, float b, float a = 1.f) { Setf(r,g,b,a); }
		void Set(u32 color) { m_Color = color; }
		void Set(int r, int g, int b, int a = 255) { SetBytes(r, g, b, a); }
		void Setf(float r, float g, float b, float a = 1.f) { Set(int(r * 255.0f), int(g * 255.0f), int(b * 255.0f), int(a * 255.0f)); }
		void SetBytes(u8 r, u8 g, u8 b, u8 a = 255) { m_Color = (r << s_ChannelShiftR) | (g << s_ChannelShiftG) | (b << s_ChannelShiftB) | (a << s_ChannelShiftA); }
		void SetRed(int r) { m_Color = (m_Color & ~s_ChannelMaskR) | (u8(r) << s_ChannelShiftR); }
		void SetGreen(int g) { m_Color = (m_Color & ~s_ChannelMaskG) | (u8(g) << s_ChannelShiftG); }
		void SetBlue(int b) { m_Color = (m_Color & ~s_ChannelMaskB) | (u8(b) << s_ChannelShiftB); }
		void SetAlpha(int a) { m_Color = (m_Color & ~s_ChannelMaskA) | (u8(a) << s_ChannelShiftA); }
		u8 GetRed() const { return u8(m_Color >> s_ChannelShiftR); }
		u8 GetGreen() const { return u8(m_Color >> s_ChannelShiftG); }
		u8 GetBlue() const { return u8(m_Color >> s_ChannelShiftB); }
		u8 GetAlpha() const { return u8(m_Color >> s_ChannelShiftA); }
		float GetRedf() const { return GetRed() * (1.0f / 255.0f); }
		float GetGreenf() const { return GetGreen() * (1.0f / 255.0f); }
		float GetBluef() const { return GetBlue() * (1.0f / 255.0f); }
		float GetAlphaf() const { return GetAlpha() * (1.0f / 255.0f); }
		u32 GetColor() const { return m_Color; }
		u32 GetDeviceColor() const { return (GetRed()) | (GetGreen() << 8) | (GetBlue() << 16) | (GetAlpha() << 24); }
		void SetFromDeviceColor(u32 colorDC) {
			Set((colorDC & s_ChannelMaskB), (colorDC & s_ChannelMaskG) >> s_ChannelShiftG, (colorDC & s_ChannelMaskR) >> s_ChannelShiftR, (colorDC & s_ChannelMaskA) >> s_ChannelShiftA);
		}
		Color32 operator*(const Color32& c) const {
			return Color32((GetRed() * c.GetRed()) / 255, (GetGreen() * c.GetGreen()) / 255, (GetBlue() * c.GetBlue()) / 255, (GetAlpha() * c.GetAlpha()) / 255);
		}
		void operator+=(const Color32& c) {
			Set(GetRed() + c.GetRed(), GetGreen() + c.GetGreen(), GetBlue() + c.GetBlue(), GetAlpha() + c.GetAlpha());
		}
		bool operator==(const Color32& c) const {
			return m_Color == c.m_Color;
		}
		bool operator!=(const Color32& c) const {
			return m_Color != c.m_Color;
		}
		static constexpr int s_ChannelShiftA = 24;
		static constexpr int s_ChannelShiftR = 16;
		static constexpr int s_ChannelShiftG = 8;
		static constexpr int s_ChannelShiftB = 0;
		static constexpr u32 s_ChannelMaskA = 0xFF000000;
		static constexpr u32 s_ChannelMaskR = 0x00FF0000;
		static constexpr u32 s_ChannelMaskG = 0x0000FF00;
		static constexpr u32 s_ChannelMaskB = 0x000000FF;
		static constexpr u32 s_ChannelMaskRGB = s_ChannelMaskR | s_ChannelMaskG | s_ChannelMaskB;
	private:
		u32 m_Color;
	};
	struct grcImage {
		u16 m_Width, m_Height;
		Format m_Format;
		ImageType m_Type;
		u16 m_Stride;
		u8 m_Depth;
		u8 m_StrideHi;
		u8* m_Bits;
		Color32* m_Lut;
		grcImage* m_Next;
		grcImage* m_NextLayer;
		int m_RefCount;
		int pad0, pad1, pad2;
		// For some fucking retarded reason, Vector3 is represented as a v4 (aka a __m128)
		__m128 m_ColorExp;
		__m128 m_ColorOfs;
	};
	enum grcMrt {
		grcmrtColor0, grcmrtColor1, grcmrtColor2, grcmrtColor3, grcmrtColor4, grcmrtColor5, grcmrtColor6, grcmrtColor7, grcmrtColorCount,
		grcmrtDepthStencil = grcmrtColorCount,
		grcmrtCount
	};
	struct grcResolveFlags {
		float Depth;
		float BlurKernelSize; // size of blur
		Color32 Color;
		u32 Stencil;
		int ColorExpBias;
		bool ClearColor;
		bool ClearDepthStencil;
		bool BlurResult;
		bool NeedResolve;
		bool MipMap;
	};
	typedef grcResolveFlags* grcResolveFlagsMrt[grcmrtColorCount];
	class grcTexture : public pgBase {
	public:
		const char* GetName() const { return m_Name; }
		virtual void Download() const {}
		virtual int GetDownloadSize() const { return 0; }
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetDepth() const = 0;
		virtual int GetMipMapCount() const = 0;
		virtual int GetBitsPerPixel() const = 0;
		virtual bool IsValid() const { return false; }
		virtual grcDevice::MSAAMode GetMSAA() const { return grcDevice::MSAA_None; }
		virtual bool IsGammaEnabled() const { return false; }
		virtual void SetGammaEnabled(bool /*enabled*/) {}
		virtual u32 GetTextureSignedMask() const { return 0; }
		virtual void SetTextureSignedMask(u32 /*mask*/) {}
		virtual grcTexture* GetReference() = 0;
		virtual bool IsSRGB() const { return false; }
		virtual const grcTexture* GetReference() const = 0;
		virtual grcTextureObject* GetTexturePtr() { return NULL; }
		virtual const grcTextureObject* GetTexturePtr() const { return NULL; }
		virtual grcDeviceView* GetTextureView() { return NULL; }
		virtual void UpdateGPUCopy() {}
		virtual u32 GetImageFormat() const { return 0; } // cast this to grcImage::Format
		grcTextureObject* GetCachedTexturePtr() const {
			if (!this)
				return NULL;
			return const_cast<grcTextureObject*>(GetTexturePtr());
		}
		virtual bool LockRect(int /*layer*/, int /*mipLevel*/, grcTextureLock&/*lock*/, u32 /*uLockFlags*/ = (grcsRead | grcsWrite)) const { return false; }
		virtual void UnlockRect(const grcTextureLock&/*lock*/) const {}
		virtual void SwizzleTexture2D(const grcTextureLock&/*lock*/) const {}
		virtual void UnswizzleTexture2D(const grcTextureLock&/*lock*/) const {}
		virtual void Resize(u32 /*width*/, u32 /*height*/) {}
		virtual void SetEffectInfo(grcEffect*, grcEffectVar) const {}
		virtual void CreateMipMaps(const grcResolveFlags* /*resolveFlags*/, int /*index*/ = 0) {};
		virtual void Blur(const grcResolveFlags* /*resolveFlags*/) {};
		virtual bool Copy(const grcImage* pImage) = 0;
		virtual bool Copy(const grcTexture* /*pTexture*/, s32 /*dstSliceIndex*/ = -1, s32 /*dstMipIndex*/ = -1, s32 /*srcSliceIndex*/ = 0, s32 /*srcMipIndex*/ = 0) { return false; }
		virtual bool Copy(const void* /*pvSrc*/, u32 /*uWidth*/, u32 /*uHeight*/, u32 /*uDepth*/) { return false; }
		virtual bool Copy2D(const void* /*pSrc*/, u32 /*imgFormat*/, u32 /*uWidth*/, u32 /*uHeight*/, u32 /*numMips*/) { return false; }
		virtual bool Copy2D(const void* /*pSrc*/, const grcPoint& /*oSrcDim*/, const grcRect& /*oDstRect*/, const grcTextureLock& /*lock*/, s32 /*iMipLevel*/ = 0) { return false; }
		virtual void Tile(int) {}
		grcCellGcmTextureWrapper m_Texture;
		const char* m_Name;
		u16 m_RefCount;
		u8 m_ResourceTypeAndConversionFlags;
		u8 m_LayerCount;
		datPadding64(4, m_Padding); // This is actually in the source, and I wish it wasn't...
		grcTextureObject* m_CachedTexturePtr;
		u32	m_PhysicalSizeAndTemplateType;
		u32	m_HandleIndex;
	};
	enum grcRenderTargetType {
		grcrtFrontBuffer,
		grcrtBackBuffer,
		grcrtDepthBuffer,
		grcrtPermanent,
		grcrtCubeMap,
		grcrtArrayMap = grcrtCubeMap,
		grcrtShadowMap,
		grcrtDepthBufferCubeMap,
		grcrtVolume,
		grcrtCount
	};
	class grcRenderTarget : public grcTexture {
	public:
		virtual ~grcRenderTarget() = default;
		virtual void Lock() {}
		virtual void Unlock() {}
		virtual void Realize(const grcResolveFlags* /*flags*/ = NULL, int /*index*/ = 0) {}
		virtual void Unrealize() {}
		virtual void AllocateMemoryFromPool() {}
		virtual void ReleaseMemoryToPool() {}
		virtual void Untile(int) {}
		virtual const grcRenderTarget* GetAABufferPtr() const { return NULL; }
		virtual grcRenderTarget* GetAABufferPtr() { return NULL; }
		virtual u8 GetSurfaceFormat() const { return 0; }
		virtual u32 GetRequiredMemory() const { return 0; }
		grcRenderTargetType GetType() const { return m_Type; }
		virtual bool IsSwizzled() const { return false; }
		virtual bool Copy(const grcTexture* /*pTexture*/, s32 /*dstSliceIndex*/ = -1, s32 /*dstMipIndex*/ = -1, s32 /*srcSliceIndex*/ = 0, s32 /*srcMipIndex*/ = 0) { return false; }
		virtual bool IsValid() const { return false; }
		virtual void UpdateMemoryLocation(const grcTextureObject*) {}
		virtual bool SaveTarget(const char* pszOutName = NULL) const { return false; }
		virtual void GenerateMipmaps() = 0;

		grcRenderTargetType	m_Type;
		u16 m_PoolID;
		u8 m_PoolHeap;
		bool m_Allocated;	
		int m_OffsetInPool;
		u8	m_ArraySize;
		u8	m_PerArraySliceRTV;
	};
	struct grcImage;
	class grcTextureFactory {
	public:
		struct TextureCreateParams {
			enum Memory_t { SYSTEM, VIDEO, STAGING }; // System indicates Dynamic CPU Write Access, Staging indicates CPU Read Access
			enum Format_t { LINEAR, TILED };
			enum Type_t { NORMAL, RENDERTARGET, REFERENCE, DICTIONARY_REFERENCE };
			enum MSAA_t { MSAA_NONE, MSAA_2X, MSAA_4X, MSAA_8X };
			enum ThreadUseHint_t { THREAD_USE_HINT_NONE, THREAD_USE_HINT_CAN_BE_UPDATE_THREAD };
			Memory_t Memory;
			Format_t Format;
			u32 LockFlags;
			void* Buffer;//client managed buffer to hold the texture data
			Type_t Type;
			MSAA_t MSAA_Type;
			grcDevice::Stereo_t StereoRTMode;
			u32 MipLevels;
			ThreadUseHint_t ThreadUseHint;
		};
		virtual ~grcTextureFactory() = default;
		virtual grcTexture* Create(const char* filename, TextureCreateParams* params = NULL) = 0;
		virtual grcTexture* Create(grcImage*, TextureCreateParams* params = NULL) = 0;
		virtual grcTexture* Create(u32 /*width*/, u32 /*height*/, u32 /*eFormat*/, void* /*pBuffer*/, u32 /*numMips*/ = 1, TextureCreateParams* /*params*/ = NULL) { return NULL; }
		virtual grcTexture* Create(u32 /*width*/, u32 /*height*/, u32 /*depth*/, u32 /*eFormat*/, void* /*pBuffer*/, TextureCreateParams* /*params*/ = NULL) { return NULL; }
		virtual u32 GetTextureDataSize(u32 /*width*/, u32 /*height*/, u32 /*format*/, u32 /*mipLevels*/, u32 /*numSlices*/, bool /*bIsCubeMap*/, bool /*bIsLinear*/, bool /*bLocalMemory*/) { return 0U; };
		virtual const grcRenderTarget* GetBackBuffer(bool realize = true) const = 0;
		virtual grcRenderTarget* GetBackBuffer(bool realize = true) = 0;
		virtual const grcRenderTarget* GetFrontBuffer(bool nextBuffer = false) const = 0;
		virtual grcRenderTarget* GetFrontBuffer(bool nextBuffer = false) = 0;
		virtual const grcRenderTarget* GetFrontBufferDepth(bool realize = true) const = 0;
		virtual grcRenderTarget* GetFrontBufferDepth(bool realize = true) = 0;
		virtual const grcRenderTarget* GetBackBufferDepth(bool realize = true) const = 0;
		virtual grcRenderTarget* GetBackBufferDepth(bool realize = true) = 0;
		virtual const grcRenderTarget* GetBackBufferDepthForceRealize() = 0;
		virtual bool CanGetBackBufferDepth() = 0;
		virtual bool CanGetBackBuffer() = 0;
		struct CreateParams {}; // I am not implementing this, go fuck yourself
		virtual grcRenderTarget* CreateRenderTarget(const char* name, grcRenderTargetType type, int width, int height, int bitsPerPixel, CreateParams* params = NULL, grcRenderTarget* originalTexture = NULL) = 0;
		virtual grcRenderTarget* CreateRenderTarget(const char* pName, const grcTextureObject* pTexture, grcRenderTarget* originalTarget = NULL) = 0;
		virtual void LockRenderTarget(int index, const grcRenderTarget* color, const grcRenderTarget* depth, u32 layer = 0, bool lockDepth = true, u32 mipToLock = 0) = 0;
		virtual void UnlockRenderTarget(int index, const grcResolveFlags* flags = NULL) = 0;
		virtual void SetArrayView(u32 uArrayIndex) { (void)uArrayIndex; }
		virtual void LockMRT(const grcRenderTarget* color[grcmrtColorCount], const grcRenderTarget* depth, const u32* mipsToLock = NULL) = 0;
		virtual void UnlockMRT(const grcResolveFlagsMrt* resolveFlags = NULL) = 0;
		virtual void SetTextureLod(float /*zDist*/) {}
		virtual void PlaceTexture(class datResource& rsc, grcTexture& tex) = 0;
		virtual void ClearFreeList() {}
		virtual void SetDefaultRenderTarget(grcMrt mrt, grcRenderTarget* renderTarget) = 0;
		virtual grcRenderTarget* GetDefaultRenderTarget(grcMrt mrt) = 0;
		virtual void PushDefaultRenderTarget(grcMrt mrt, grcRenderTarget* renderTarget) = 0;
		virtual grcRenderTarget* PopDefaultRenderTarget(grcMrt mrt) = 0;
		virtual void BindDefaultRenderTargets() = 0;
	};
}
//strXXXXXXX
namespace rage
{
	enum eHierarchyModType
	{
		HMT_ENABLE = 0,
		HMT_DISABLE,
		HMT_FLUSH
	};
#define STRINDEX_INVALID 0xFFFFFFFF
#define STRLOCALINDEX_INVALID 0xFFFFFFFF
	template <typename T>
	struct strDynamicIndex
	{
		enum eInvalidIndex
		{
			INVALID_INDEX = STRLOCALINDEX_INVALID
		};
		inline T Get() const
		{
			return m_Index;
		}
		inline void Set(T i)
		{
			m_Index = i;
		}
		inline bool IsValid() const
		{
			return m_Index != STRLOCALINDEX_INVALID;
		}
		inline bool IsInvalid() const
		{
			return m_Index == STRLOCALINDEX_INVALID;
		}
		inline operator T()
		{
			return Get();
		}
		inline void operator =(const T other)
		{
			Set(other);
		}
		inline void operator =(const strDynamicIndex<T>& other)
		{
			m_Index = other.m_Index;
		}
		inline bool operator ==(const strDynamicIndex<T>& other) const
		{
			return m_Index == other.m_Index;
		}
		inline bool operator ==(const T& other) const
		{
			return m_Index == other;
		}
		inline bool operator !=(const strDynamicIndex<T>& other) const
		{
			return m_Index != other.m_Index;
		}
		inline bool operator !=(const T& other) const
		{
			return m_Index != other;
		}
		inline bool operator >(const strDynamicIndex<T>& other) const
		{
			return m_Index > other.m_Index;
		}
		inline bool operator >(const T& other) const
		{
			return m_Index > other;
		}
		inline bool operator >=(const strDynamicIndex<T>& other) const
		{
			return m_Index >= other.m_Index;
		}
		inline bool operator >=(const T& other) const
		{
			return m_Index >= other;
		}
		inline bool operator <=(const strDynamicIndex<T>& other) const
		{
			return m_Index <= other.m_Index;
		}
		inline bool operator <=(const T& other) const
		{
			return m_Index <= other;
		}
		inline bool operator <(const strDynamicIndex<T>& other) const
		{
			return m_Index < other.m_Index;
		}
		inline bool operator <(const T& other) const
		{
			return m_Index < other;
		}
		T m_Index;
	};
	typedef strDynamicIndex<u32> strIndex;
	typedef strDynamicIndex<s32> strLocalIndex;
	typedef atHashString strStreamingObjectName;
	typedef atFinalHashString strStreamingObjectNameString;
	enum strRefKind
	{
		REF_RENDER = 0,
		REF_SCRIPT = 1,
		REF_DEFRAG = REF_RENDER,
		REF_OTHER = 2,
		MAX_REF_KINDS
	};
	class strStreamingLoader
	{
	public:
		enum Status
		{
			STATUS_IDLE, STATUS_LOADING, STATUS_CANCELED, STATUS_MAX_VALUE
		};
		struct StreamingFile
		{
			// Max value is 2, but we need to give it an extra bit
			// since enums are signed! (We could handle it in code,
			// but this approach ends up with less confusion.)
			Status m_Status : 3;
			u32 m_IsPlaced : 1;			// Set to true after the placement thread finishes placing it
			u32 m_Reissue : 1;			// Second time we're trying to place this
			s32 m_LoadTime : 27;
			strIndex m_Index;
			sysIpcSema m_ReadSema;
			pgStreamer::Handle m_Handle;
			pgStreamer::ReadId m_ReadId;
			datResourceMap m_LoadingMap;
			datResourceMap m_AllocationMap; // A superset of m_LoadingMap
		};
	};
	class strStreamingModule : public datBase
	{
		friend class strStreamingModuleMgr;
	public:
		virtual ~strStreamingModule()
		{}
		const char* GetModuleName() const
		{
			return m_ModuleName.c_str();
		}
		virtual const char* GetName(strLocalIndex UNUSED_PARAM(index)) const
		{
			return m_ModuleName.c_str();
		}
		virtual strLocalIndex Register(const char* name) = 0;
		virtual strLocalIndex FindSlot(const char* name) const = 0;
		virtual void Remove(strLocalIndex index)
		{}
		virtual void RemoveSlot(strLocalIndex UNUSED_PARAM(index))
		{}
		virtual bool Load(strLocalIndex UNUSED_PARAM(index), void* UNUSED_PARAM(object), int UNUSED_PARAM(size))
		{
			return false;
		}
		virtual void PlaceResource(strLocalIndex UNUSED_PARAM(index), datResourceMap& UNUSED_PARAM(map), datResourceInfo& UNUSED_PARAM(header))
		{}
		virtual void SetResource(strLocalIndex UNUSED_PARAM(index), datResourceMap& UNUSED_PARAM(map))
		{}
		virtual void* GetPtr(strLocalIndex UNUSED_PARAM(index))
		{
			return NULL;
		}
		virtual void* GetDataPtr(strLocalIndex index)
		{
			return GetPtr(index);
		}
		virtual void* Defragment(strLocalIndex UNUSED_PARAM(index), datResourceMap& UNUSED_PARAM(map), bool& UNUSED_PARAM(flush))
		{
			return NULL;
		}
		virtual void DefragmentComplete(strLocalIndex UNUSED_PARAM(index))
		{}
		virtual void DefragmentPreprocess(strLocalIndex UNUSED_PARAM(index))
		{}
		virtual pgBase* GetResource(strLocalIndex UNUSED_PARAM(index)) const
		{
			return NULL;
		}
		virtual void GetModelMapTypeIndex(strLocalIndex UNUSED_PARAM(slotIndex), strIndex& UNUSED_PARAM(retIndex)) const
		{}
		virtual bool ModifyHierarchyStatus(strLocalIndex UNUSED_PARAM(index), eHierarchyModType UNUSED_PARAM(modType))
		{
			return false;
		}
		virtual void AddRef(strLocalIndex UNUSED_PARAM(index), strRefKind UNUSED_PARAM(strRefKind))
		{}
		virtual void RemoveRef(strLocalIndex UNUSED_PARAM(index), strRefKind UNUSED_PARAM(strRefKind))
		{}
		virtual void ResetAllRefs(strLocalIndex UNUSED_PARAM(index))
		{}
		virtual int GetNumRefs(strLocalIndex UNUSED_PARAM(index)) const
		{
			return 1;
		}
		virtual const char* GetRefCountString(strLocalIndex index, char* buffer, u64 bufferLen) const = 0;
		virtual int GetDependencies(strLocalIndex UNUSED_PARAM(index), strIndex* UNUSED_PARAM(pIndices), int UNUSED_PARAM(indexArraySize)) const
		{
			return 0;
		}
		virtual void PrintExtraInfo(strLocalIndex UNUSED_PARAM(index), char* UNUSED_PARAM(extraInfo), u64 UNUSED_PARAM(maxSize)) const
		{}
		virtual void RequestExtraMemory(strLocalIndex UNUSED_PARAM(index), datResourceMap& UNUSED_PARAM(map), int UNUSED_PARAM(maxAllocs))
		{}
		virtual void ReceiveExtraMemory(strLocalIndex UNUSED_PARAM(index), const datResourceMap& UNUSED_PARAM(map))
		{}
		virtual u64 GetExtraVirtualMemory(strLocalIndex UNUSED_PARAM(index)) const
		{
			return 0;
		}
		virtual u64 GetExtraPhysicalMemory(strLocalIndex UNUSED_PARAM(index)) const
		{
			return 0;
		}
		virtual bool IsDefragmentCopyBlocked() const
		{
			return false;
		}
		virtual bool RequiresTempMemory(strLocalIndex UNUSED_PARAM(objIndex)) const
		{
			return m_RequiresTempMemory;
		}
		virtual bool CanPlaceAsynchronously(strLocalIndex UNUSED_PARAM(objIndex)) const
		{
			return false;
		}
		virtual void PlaceAsynchronously(strLocalIndex UNUSED_PARAM(objIndex), strStreamingLoader::StreamingFile& UNUSED_PARAM(file), datResourceInfo& UNUSED_PARAM(info))
		{}
		strIndex m_indexOffset;
		s32 m_moduleId;
		u32 m_size;
		atString m_ModuleName;
		bool m_RequiresTempMemory;
		bool m_canDefragment;
		bool m_usesExtraMemory;
		s32 m_expectedRscVersionNumber;
		s32 m_moduleTypeIndex;
	};
	class strStreamingModuleMgr : public datBase
	{
	private:
		struct ModuleIndexInfo
		{
			strStreamingModule* m_Module;
			strIndex m_LastIndex;
			int m_ModuleIndex;
		};
	public:
		strStreamingModule* GetModule(s32 module)
		{
			return m_moduleArray[module];
		}
		const strStreamingModule* GetModule(s32 module) const
		{
			return m_moduleArray[module];
		}
		int GetNumberOfModules() const
		{
			return m_moduleArray.size();
		}
		strStreamingModule* GetModuleFromIndex(strIndex streamingIndex) const
		{
			const ModuleIndexInfo* info = GetModuleInfoFromIndex(streamingIndex);
			return info->m_Module;
		}
		u8 GetModuleIdFromIndex(strIndex streamingIndex) const
		{
			const ModuleIndexInfo* info = GetModuleInfoFromIndex(streamingIndex);
			return (u8)info->m_ModuleIndex;
		}
		atArray<ModuleIndexInfo> m_ModuleIndexRanges;
		atArray<strStreamingModule*> m_moduleArray;
	private:
		const ModuleIndexInfo* GetModuleInfoFromIndex(strIndex index) const
		{
			int moduleCount = m_ModuleIndexRanges.size();
			int minValue = 0, maxValue = moduleCount - 1;
			int current = maxValue >> 1;
			const ModuleIndexInfo* indexInfos = m_ModuleIndexRanges.data();
			while (true)
			{
				const ModuleIndexInfo* info = &indexInfos[current];
				if (index < info->m_LastIndex)
				{
					// We need to move left, or we found the right one.
					if (minValue == current)
					{
						return info;
					}
					maxValue = current;
					int newCurrent = (minValue + maxValue) >> 1;
					if (newCurrent == current)
					{
						newCurrent--;
					}
					current = newCurrent;
				}
				else
				{
					// We have to move to the right.
					minValue = current + 1;
					if (minValue > maxValue)
					{
						return NULL;
					}
					int newCurrent = (minValue + maxValue) >> 1;
					if (newCurrent == current)
					{
						newCurrent++;
					}
					current = newCurrent;
				}
			}
		}
	};
}
//fwXXXXXXX
namespace rage
{
	class fwExtensibleBase; // Fuck this impl right now
	template<typename _KeyType>
	class fwNameRegistrarDef
	{
	public:
		int Lookup(_KeyType key) const
		{
			u32 bucket = m_Hash[key % m_HashCount];
			while (bucket != NONE)
			{
				const Bucket& b = m_Buckets[bucket];
				if (b.hash != key)
				{
					bucket = b.next;
				}
				else
				{
					return b.value;
				}
			}
			return -1;
		}
		void Insert(_KeyType key, u32 value)
		{
			if (m_FirstFreeBucket == NONE)
			{
				return;
			}
			u32 hashIdx = key % m_HashCount;
			Bucket& n = m_Buckets[m_FirstFreeBucket];
			// Remember next free bucket
			u32 nextFree = n.next;
			// Update the new entry, patched into current chain
			n.hash = key;
			n.value = value;
			n.next = m_Hash[hashIdx];
			// Patch head of chain to this entry
			m_Hash[hashIdx] = m_FirstFreeBucket;
			// Update new free bucket.
			m_FirstFreeBucket = nextFree;
			m_entries++;
		}
		void Delete(_KeyType key)
		{
			u32 hashIdx = key % m_HashCount;
			u32* prevBucket = &m_Hash[hashIdx];
			u32 bucket = m_Hash[hashIdx];
			while (bucket != NONE)
			{
				Bucket& b = m_Buckets[bucket];
				if (b.hash == key)
				{
					m_entries--;
					// Patch self out of chain; this may mark the entire chain free.
					*prevBucket = b.next;
					b.hash = (_KeyType)0;
					b.value = 0;
					b.next = m_FirstFreeBucket;
					m_FirstFreeBucket = bucket;
					return;
				}
				else
				{
					prevBucket = &b.next;
					bucket = b.next;
				}
			}
		}
		bool IsInitialized() const
		{
			return m_Hash && m_Buckets;
		}
		u32 GetCount()
		{
			return m_entries;
		}
	private:
		static constexpr u32 NONE = 0xFFFFFFFF;
		struct Bucket
		{
			_KeyType hash; u32 value; u32 next;
		};
		Bucket* m_Buckets;
		u32* m_Hash;
		u32 m_HashCount, m_FirstFreeBucket;
		u32 m_entries;
	};
	typedef fwNameRegistrarDef<u32> fwNameRegistrar;
	#define POOL_FLAG_ISFREE 0x80
	#define POOL_FLAG_REFERENCEMASK 0x7F
	class fwBasePool
	{
	public:
		void* New()
		{
			if (m_nFirstFreeIndex == -1)
			{
				return NULL;
			}
			s32 freeIndex = m_nFirstFreeIndex;
			s32* nextFree = (s32*)(m_aStorage + freeIndex * m_nStorageSize);
			m_nFirstFreeIndex = *nextFree;
			if (m_nFirstFreeIndex == -1)
				m_nLastFreeIndex = -1;
			void* result = (void*)nextFree;
			SetIsFree(freeIndex, false);
			SetReference(freeIndex, GetReference(freeIndex) + 1);
			m_numSlotsUsed++;
			return result;
		}
		void* New(s32 index)
		{
			const u32 i = (index >> 8);
			SetIsFree(i, false);
			SetReference(i, (u8)(index & 255));
			u8* pT = &m_aStorage[i * m_nStorageSize];
			// Locate the object in the free list
			index >>= 8;
			s32* prev = &m_nFirstFreeIndex;
			while (*prev != -1 && *prev != index)
				prev = (s32*)(m_aStorage + *prev * m_nStorageSize);
			// Patch the free list around the item we just allocated
			*prev = *(s32*)(m_aStorage + index * m_nStorageSize);
			// If this item was last on the free list, update that too
			if (m_nLastFreeIndex == index)
				m_nLastFreeIndex = static_cast<s32>((prev == &m_nFirstFreeIndex) ? -1 : ((u8*)prev - m_aStorage) / m_nStorageSize);
			m_numSlotsUsed++;
			return (void*)pT;
		}
		typedef bool (*Callback)(void* item, void* data);
		void ForAll(Callback cb, void* data)
		{
			s32 size = GetSize();
			while (size--)
			{
				void* pItem = GetSlot(size);
				if (pItem)
				{
					if (!cb(pItem, data))
						return;
				}
			}
		}
		/*__forceinline void* GetSlot(s32 index)
		{
			return const_cast<void*>(reinterpret_cast<fwBasePool*>(this)->GetSlot(index));
		}*/
		void* GetAt(s32 index)
		{
			const u32 i = (index >> 8);
			return m_aFlags[i] == (index & 0xFF) ? (void*)&m_aStorage[i * m_nStorageSize] : nullptr;
		}
		// returns (x ? ~0 : 0) without branching
		static __forceinline u64 GetNonZeroMask(u32 x)
		{
			return (u64)(((s64)(x) | -(s64)(x)) >> (sizeof(u64) * 8 - 1));
		}
		__forceinline void* GetSlot(s32 index)
		{
			const u64 mask = ~GetNonZeroMask(m_aFlags[index] & POOL_FLAG_ISFREE);
			const u64 addr = reinterpret_cast<u64>(m_aStorage + index * m_nStorageSize);
			void* result = reinterpret_cast<void*>(addr & mask);
			return result;
		}
		void SetIsFree(s32 index, bool bIsFree)
		{
			bIsFree ? (m_aFlags[index] |= POOL_FLAG_ISFREE) : (m_aFlags[index] &= ~POOL_FLAG_ISFREE);
		}
		void SetReference(s32 index, u8 nReference)
		{
			m_aFlags[index] = (m_aFlags[index] & ~POOL_FLAG_REFERENCEMASK) | (((nReference & POOL_FLAG_REFERENCEMASK) > 1 ? (nReference & POOL_FLAG_REFERENCEMASK) : 1));
		}
		u64 GetStorageSize() const
		{
			return m_nStorageSize;
		}
		s32 GetSize() const
		{
			return m_nSize;
		}
		s32 GetNoOfUsedSpaces() const
		{
			return m_numSlotsUsed;
		}
		s32 GetNoOfFreeSpaces() const
		{
			return m_nSize - m_numSlotsUsed;
		}
		s32 GetPoolSize() const
		{
			return m_nSize;
		}
		bool GetIsFree(s32 index) const
		{
			return (m_aFlags[index] & POOL_FLAG_ISFREE) != 0;
		}
		u8 GetReference(s32 index) const
		{
			return (m_aFlags[index] & POOL_FLAG_REFERENCEMASK);
		}
		u8 GetFlags(s32 index) const
		{
			return (m_aFlags[index]);
		}
		u8* m_aStorage;
		u8* m_aFlags;
		s32 m_nSize;
		s32 m_nStorageSize;
		s32 m_nFirstFreeIndex;
		s32 m_nLastFreeIndex;
		s32 m_numSlotsUsed : 30;
		s32 m_bOwnsArrays : 2;
	};
	template <class T>
	class fwPool : public fwBasePool
	{
	public:
		T* New()
		{
			return static_cast<T*>(fwBasePool::New());
		}
		T* New(s32 index)
		{
			return static_cast<T*>(fwBasePool::New(index));
		}
		T* GetAt(s32 index)
		{
			return static_cast<T*>(fwBasePool::GetAt(index));
		}
		T* GetSlot(s32 index)
		{
			return static_cast<T*>(fwBasePool::GetSlot(index));
		}
		const T* GetSlot(s32 index) const
		{
			return static_cast<const T*>(fwBasePool::GetSlot(index));
		}
		// DO NOT CALL THIS, it links to a pointer in the game, and we just used a recreation. It SHOULD call delete inside rage, but it does NOT.
		// It WILL crash
		void DeleteAll()
		{
			ForAll(DeleteAllCallback, NULL);
		}
		static bool DeleteAllCallback(void* item, void* /*data*/)
		{
			delete (T*)item; return true;
		}
	};
	template <typename T, typename S>
	class fwGenericAssetDef
	{
	public:
		typedef S strObjectNameType;
		T* m_pObject;
		int m_refCount;
		S m_name;
	};
	typedef pgDictionary<grcTexture> fwTxd;
	template <typename T>
	class fwAssetDef : public fwGenericAssetDef<T, strStreamingObjectName>
	{};
	template <typename T>
	class fwAssetNameDef : public fwGenericAssetDef<T, strStreamingObjectNameString>
	{};
	class fwAssetStoreBase : public strStreamingModule
	{
	public:
		virtual s32 GetSize() const
		{
			return 0;
		}
		virtual s32 GetNumUsedSlots() const
		{
			return 0;
		}
	};
	template <typename T, typename S = fwAssetDef<T>>
	class fwAssetStore : public fwAssetStoreBase
	{
	public:
		typedef fwPool<S> Pool;
		virtual ~fwAssetStore() = default;
		strStreamingModule* GetStreamingModule()
		{
			return this;
		}
		virtual void Shutdown() = 0;
		virtual void* GetPtr(strLocalIndex index) = 0;
		virtual strLocalIndex Register(const char* name) = 0;
		virtual void Remove(strLocalIndex index) = 0;
		virtual bool Load(strLocalIndex index, void* pData, int size) = 0;
		// reference counting
		virtual void AddRef(strLocalIndex index, strRefKind strRefKind) = 0;
		virtual void RemoveRef(strLocalIndex index, strRefKind strRefKind) = 0;
		virtual void RemoveRefWithoutDelete(strLocalIndex index, strRefKind strRefKind) = 0;
		virtual void ResetAllRefs(strLocalIndex index) = 0;
		virtual int GetNumRefs(strLocalIndex index) const = 0;
		virtual bool LoadFile(strLocalIndex index, const char* pFilename) = 0;
		virtual void Set(strLocalIndex index, T* m_pObject) = 0;
		// add remove slots
		virtual strLocalIndex AddSlot(const typename S::strObjectNameType name) = 0;
		virtual strLocalIndex AddSlot(strLocalIndex index, const typename S::strObjectNameType name) = 0;
		virtual void RemoveSlot(strLocalIndex index) = 0;
		Pool m_pool;
		atString m_fileExt;
		fwNameRegistrar m_reg;
		bool m_SizeFinalized;

		void InsertHashKey(u32 key, int idx)
		{
			m_reg.Insert(key, idx);
		}
		void RemoveHashKey(u32 key)
		{
			m_reg.Delete(key);
		}
		S* GetSlot(const char* name)
		{
			strLocalIndex index{ FindSlot(name) };
			return m_pool.GetSlot(index.Get());
		}
		S* GetSlot(strLocalIndex index)
		{
			return m_pool.GetSlot(index.Get());
		}
		T* Get(strLocalIndex index)
		{
			S* pDef = GetSlot(index);
			return pDef->m_pObject;
		}
		T* Get(const char* name)
		{
			S* pDef = GetSlot(name);
			return pDef->m_pObject;
		}
	};
	constexpr int class_fw = offsetof(fwAssetStore<void*>, m_pool);
	template <typename T, typename S = fwAssetDef<T>>
	class fwAssetRscStore : public fwAssetStore<T, S>
	{
	public:
		virtual bool LoadFile(strLocalIndex index, const char* pFilename) = 0;
		virtual void Set(strLocalIndex index, T* m_pObject) = 0;
		virtual void Remove(strLocalIndex index) = 0;
		virtual void PlaceResource(strLocalIndex index, datResourceMap& map, datResourceInfo& header) = 0;
		virtual void SetResource(strLocalIndex index, datResourceMap& map) = 0;
		virtual void* Defragment(strLocalIndex index, datResourceMap& map, bool& flush) = 0;
		virtual bool CanPlaceAsynchronously(strLocalIndex UNUSED_PARAM(objIndex)) const
		{
			return true;
		}
		virtual void PlaceAsynchronously(strLocalIndex objIndex, strStreamingLoader::StreamingFile& file, datResourceInfo& rsc) = 0;
		virtual pgBase* GetResource(strLocalIndex index) const = 0;
	};
	class fwTxdDef : public fwAssetDef<fwTxd>
	{
	public:
		strLocalIndex GetParentId() const
		{
			return strLocalIndex(m_parentId);
		}
		s32	m_parentId;
		u16 m_isDummy : 1;
		u16 m_bIsBeingDefragged : 1;
		u16 m_bHasHD : 1;
		u16 m_bIsBoundHD : 1;
		u16 m_padding : 12;
	};
	class fwTxdStore : public fwAssetRscStore<fwTxd, fwTxdDef>
	{
	public:
		virtual void Set(strLocalIndex index, fwTxd* m_pObject) = 0;
		virtual void Remove(strLocalIndex index) = 0;
		virtual void RemoveSlot(strLocalIndex index) = 0;
		virtual void* Defragment(strLocalIndex index, datResourceMap& map, bool& flush) = 0;
		virtual void DefragmentComplete(strLocalIndex index) = 0;
		virtual void DefragmentPreprocess(strLocalIndex index) = 0;
		virtual int GetDependencies(strLocalIndex UNUSED_PARAM(index), strIndex* UNUSED_PARAM(pIndices), int indexArraySize) const = 0;
		virtual bool LoadFile(strLocalIndex index, const char* pFilename) = 0;
		virtual bool Load(strLocalIndex index, void* pData, int size) = 0;
		virtual strLocalIndex Register(const char* name) = 0;
		grcTexture* GetTexture(strLocalIndex index, int dictionaryEntry)
		{
			if (index.IsValid())
			{
				fwTxd* txd = Get(index);
				if (txd)
					return txd->GetEntry(dictionaryEntry);
			}
			return NULL;
		}
	};
}
//rageSec
namespace rage
{
	class gameSkeleton
	{
	public:
		typedef void (*fnInitFunction)(u32);
		typedef void (*fnShutdownFunction)(u32);
		typedef void (*fnUpdateFunction)();
		virtual ~gameSkeleton()
		{}
		void SetCurrentInitType(u32 initType)
		{
			m_CurrentInitType = initType;
		}
		void SetCurrentShutdownType(u32 shutdownType)
		{
			m_CurrentShutdownType = shutdownType;
		}
		void SetCurrentDependencyLevel(u32 dependencyLevel)
		{
			m_CurrentDependencyLevel = dependencyLevel;
		}
		virtual void Init(u32 initMode) = 0;
		virtual void Shutdown(u32 shutdownMode) = 0;
		virtual void Update(u32 updateMode) = 0;
		static constexpr u32 DEFAULT_MAX_SYSTEMS_REGISTERED = 128;
		static constexpr u32 MAX_DEPENDENCY_LEVELS = 32;
		static constexpr u32 MAX_NESTED_UPDATE_GROUPS = 32;
		struct systemData
		{
			systemData()
			{}
			bool InitialisesThisMode(u32 initMode) const
			{
				return ((m_InitTypes & initMode) != 0);
			}
			bool ShutdownsThisMode(u32 shutdownMode) const
			{
				return ((m_ShutdownTypes & shutdownMode) != 0);
			}
			fnInitFunction     m_InitFunction;
			fnShutdownFunction m_ShutdownFunction;
			u32           m_InitDependencyLevel;
			u32           m_ShutdownDependencyLevel;
			u32           m_InitTypes;
			u32           m_ShutdownTypes;
			atHashString       m_SystemName;
		};
		struct dependency
		{
			dependency(u32 level) : dependencyLevel(level), next(0)
			{}
			u32 dependencyLevel;
			atArray<u32> sysData;
			dependency* next;
		};
		struct mode
		{
			mode(u32 type) : modeType(type), head(0), next(0)
			{}
			u32 modeType;
			dependency* head;
			mode* next;
		};
		struct updateBase
		{
			updateBase(const atHashString& name, bool addTimebar, float timebarBudget) : m_AddTimebar(addTimebar), m_TimebarBudget(timebarBudget), m_Name(name), m_Next(0)
			{}
			virtual ~updateBase()
			{}
			virtual void Update() = 0;
			bool m_AddTimebar;
			float m_TimebarBudget;
			atHashString m_Name;
			updateBase* m_Next;
		};
		struct updateElement : public updateBase
		{
			updateElement() : updateBase(atHashString(), false, 0.0f)
			{}
			updateElement(const atHashString& name, bool addTimebar, float timebarBudget, fnUpdateFunction updateFunction) : updateBase(name, addTimebar, timebarBudget), m_UpdateFunction(updateFunction)
			{}
			fnUpdateFunction m_UpdateFunction;
		};
		struct updateGroup : public updateBase
		{
			updateGroup(const atHashString& name, bool addTimebar, float timebarBudget) : updateBase(name, addTimebar, timebarBudget), m_Head(0)
			{}
			~updateGroup()
			{
				updateBase* element = m_Head;
				while (element)
				{
					updateBase* next = element->m_Next;
					delete element;
					element = next;
				}
			}
			updateBase* m_Head;
		};
		struct updateMode
		{
			updateMode(u32 type) : m_ModeType(type), m_Head(0), m_Next(0)
			{}
			~updateMode()
			{
				updateBase* element = m_Head;
				while (element)
				{
					updateBase* next = element->m_Next;
					delete element;
					element = next;
				}
			}
			void Update()
			{
				updateBase* element = m_Head;
				while (element)
				{
					element->Update();
					element = element->m_Next;
				}
			}
			u32 m_ModeType;
			updateBase* m_Head;
			updateMode* m_Next;
		};
		const mode* GetMode(const mode* modeRoot, u32 modeType) const
		{
			const mode* currMode = modeRoot;
			while (currMode && currMode->modeType != modeType)
			{
				currMode = currMode->next;
			}
			return currMode;
		}
		u32 m_CurrentDependencyLevel; // The current dependency level, used when building an init/shutdown mode
		u32 m_CurrentInitType; // The current init mode type, used when building an init mode
		u32 m_CurrentShutdownType; // The current shutdown mode type, used when building an shutdown mode
		u32 m_CurrentUpdateType; // The current update mode type, used when building an update mode
		atArray<systemData> m_RegisteredSystemData; // Array of system data, used as storage for data that can be shared between init/shutdown modes
		u32 m_UpdateGroupStackPointer; // The update group stack pointer, used when building an update mode
		updateGroup* m_UpdateGroupStack[MAX_NESTED_UPDATE_GROUPS]; // The update group stack, used when building an update mode
		mode* m_InitModes; // Linked list of different registered initialisation modes
		mode* m_ShutdownModes; // Linked list of different registered shutdown modes
		updateMode* m_UpdateModes; // Linked list of different registered update modes
	};
}
//fiXXXXXXX
namespace rage {
	#define fiHandleInvalid	((fiHandle)-1)
	inline bool fiIsValidHandle(fiHandle h)
	{
		return h != fiHandleInvalid;
	}
	enum fiSeekWhence
	{
		seekSet, // Position is relative to start of file
		seekCur, // Position is relative to current file position (can be negative)
		seekEnd  // Position is relative to end of file. Reliable operation is only guaranteed if pos is zero or negative
	};
	const int RAGE_MAX_PATH = 256;
	struct fiFindData
	{
		char m_Name[RAGE_MAX_PATH];	// Name of the file
		u64 m_Size;			// Size of the file
		u64 m_LastWriteTime;// Last time the file was written
		u32 m_Attributes;	// Bitfield using the FILE_ATTRIBUTE_... values
	};
	class fiDevice
	{
	public:
		enum RootDeviceId
		{
			UNKNOWN,
			OPTICAL,
			HARDDRIVE,
			USB,
			NETWORK,
			MEMORY,
		};
		static constexpr u32 HARDDRIVE_LSN = 0x40000000;
		virtual ~fiDevice()
		{}
		virtual fiHandle Open(const char* filename, bool readOnly) const = 0;
		virtual fiHandle OpenBulk(const char* filename, u64& outBias) const = 0;
		virtual fiHandle OpenBulkDrm(const char* filename, u64& outBias, const void* pDrmKey) const = 0;
		virtual fiHandle CreateBulk(const char* filename) const = 0;
		virtual fiHandle Create(const char* filename) const = 0;
		virtual int Read(fiHandle handle, void* outBuffer, int bufferSize) const = 0;
		virtual int ReadBulk(fiHandle handle, u64 offset, void* outBuffer, int bufferSize) const = 0;
		virtual int WriteBulk(fiHandle handle, u64 offset, const void* inBuffer, int bufferSize) const = 0;
		virtual int Write(fiHandle handle, const void* buffer, int bufferSize) const = 0;
		virtual int Seek(fiHandle handle, int offset, fiSeekWhence whence) const = 0;
		virtual u64 Seek64(fiHandle handle, s64 offset, fiSeekWhence whence) const = 0;
		virtual int Close(fiHandle handle) const = 0;
		virtual int CloseBulk(fiHandle handle) const = 0;
		virtual int Size(fiHandle handle) const = 0;
		virtual u64 Size64(fiHandle handle) const = 0;
		virtual int Flush(fiHandle handle) const = 0;
		virtual bool Delete(const char* filename) const = 0;
		virtual bool Rename(const char* from, const char* to) const = 0;
		virtual bool MakeDirectory(const char* pathname) const = 0;
		virtual bool UnmakeDirectory(const char* pathname) const = 0;
		virtual void Sanitize(fiHandle handle) const = 0;
		virtual u64 GetFileSize(const char* filename) const = 0;
		virtual u64 GetFileTime(const char* filename) const = 0;
		virtual bool SetFileTime(const char* filename, u64 timestamp) const = 0;
		virtual fiHandle FindFileBegin(const char* directoryName, fiFindData& outData) const = 0;
		virtual bool FindFileNext(fiHandle handle, fiFindData& outData) const = 0;
		virtual int FindFileEnd(fiHandle handle) const = 0;
		virtual const fiDevice* GetLowLevelDevice() const
		{
			return this;
		}
		virtual char* FixRelativeName(char* dest, int destSize, const char* src) const = 0;
		virtual bool SetEndOfFile(fiHandle handle) const = 0;
		virtual u32 GetAttributes(const char* filename) const = 0;
		virtual bool PrefetchDir(const char* /*directory*/) const
		{
			return true;
		}
		virtual bool SetAttributes(const char* filename, u32 attributes) const = 0;
		virtual RootDeviceId GetRootDeviceId(const char* /*name*/) const = 0;
		virtual bool SafeRead(fiHandle handle, void* outBuffer, int size) const = 0;
		virtual bool SafeWrite(fiHandle handle, const void* buffer, int size) const = 0;
		virtual int GetResourceInfo(const char* name, void* outHeader) const = 0;
		virtual u32 GetEncryptionKey() const
		{
			return 0;
		}
		virtual u64 GetBulkOffset(const char*) const
		{
			return 0;
		}
		virtual u32 GetPhysicalSortKey(const char*) const
		{
			return HARDDRIVE_LSN;
		}
		virtual bool IsRpf() const
		{
			return false;
		}
		virtual bool IsMaskingAnRpf() const
		{
			return false;
		}
		virtual const fiDevice* GetRpfDevice() const
		{
			return this;
		}
		virtual bool IsCloud() const
		{
			return false;
		}
		virtual u32 GetPackfileIndex() const
		{
			return 0;
		}
		virtual const char* GetDebugName() const = 0;
	};
	class fiDeviceRelative : public fiDevice
	{
	public:
		virtual ~fiDeviceRelative()
		{}
		virtual fiHandle Open(const char* filename, bool readOnly) const = 0;
		virtual fiHandle OpenBulk(const char* filename, u64& bias) const = 0;
		virtual fiHandle OpenBulkDrm(const char* filename, u64& outBias, const void* pDrmKey) const = 0;
		virtual fiHandle Create(const char* filename) const = 0;
		virtual int Read(fiHandle handle, void* outBuffer, int bufferSize) const = 0;
		virtual int ReadBulk(fiHandle handle, u64 offset, void* outBuffer, int bufferSize) const = 0;
		virtual int Write(fiHandle handle, const void* buffer, int bufferSize) const = 0;
		virtual int Seek(fiHandle handle, int offset, fiSeekWhence whence) const = 0;
		virtual u64 Seek64(fiHandle handle, s64 offset, fiSeekWhence whence) const = 0;
		virtual int Close(fiHandle handle) const = 0;
		virtual int CloseBulk(fiHandle handle) const = 0;
		virtual int Size(fiHandle handle) const = 0;
		virtual u64 Size64(fiHandle handle) const = 0;
		virtual bool Delete(const char* filename) const = 0;
		virtual bool Rename(const char* from, const char* to) const = 0;
		virtual bool MakeDirectory(const char* filename) const = 0;
		virtual bool UnmakeDirectory(const char* filename) const = 0;
		virtual u64 GetFileSize(const char* filename) const = 0;
		virtual u64 GetFileTime(const char* filename) const = 0;
		virtual bool SetFileTime(const char* filename, u64 timestamp) const = 0;
		virtual fiHandle FindFileBegin(const char* filename, fiFindData& outData) const = 0;
		virtual bool FindFileNext(fiHandle handle, fiFindData& outData) const = 0;
		virtual int FindFileEnd(fiHandle handle) const = 0;
		virtual bool SetEndOfFile(fiHandle handle) const = 0;
		virtual u32 GetAttributes(const char* filename) const = 0;
		virtual bool SetAttributes(const char* filename, u32 attributes) const = 0;
		virtual bool PrefetchDir(const char* directory) const = 0;
		virtual u32 GetPhysicalSortKey(const char* filename) const = 0;
		virtual int GetResourceInfo(const char* name, datResourceInfo& info) const = 0;
		virtual u64 GetBulkOffset(const char* name) const = 0;
		virtual const char* GetDebugName() const = 0;
		virtual RootDeviceId GetRootDeviceId(const char* name) const = 0;
		virtual const fiDevice* GetLowLevelDevice() const = 0;
		virtual char* FixRelativeName(char* dest, int destSize, const char* src) const = 0;
		virtual bool IsMaskingAnRpf() const = 0;
		virtual const fiDevice* GetRpfDevice() const = 0;
	protected:
		const fiDevice* m_pDevice;
	public:
		char m_path[RAGE_MAX_PATH];
		s32 m_filenameOffset;
		bool m_IsReadOnly;
	};
	class fiDeviceCrc : public fiDeviceRelative
	{
	public:
		virtual ~fiDeviceCrc()
		{}
		virtual fiHandle Open(const char* filename, bool readOnly) const = 0;
		virtual int Read(fiHandle handle, void* outBuffer, int bufferSize) const = 0;
		virtual int Close(fiHandle handle) const = 0;
	};
	class fiStream
	{
	public:
		const fiDevice* m_Device;
		fiHandle m_Handle;
		char* m_Buffer;
		u64 m_Start;	// location of last seek
		int m_Offset,	// offset into buffer of current position
			m_Length,	// amount of info in buffer during read
			m_Size;		// total size of the buffer
	};
	struct fiPackEntry
	{
		bool IsDir() const
		{
			return m_FileOffset == FileOffset_IsDir;
		}
		bool IsResource() const
		{
			return m_IsResource;
		}
		bool IsFile() const
		{
			return !IsDir() && !IsResource();
		}
		bool IsCompressed() const
		{
			return m_ConsumedSize != 0;
		}
		u32 GetConsumedSize() const
		{
			return m_ConsumedSize ? (u32)m_ConsumedSize : u.file.m_UncompressedSize;
		}
		u32 GetUncompressedSize() const
		{
			return IsResource() ? u32(m_ConsumedSize) : u.file.m_UncompressedSize;
		}
		u32 GetFileOffset() const
		{
			return u32(m_FileOffset) << FileOffset_Shift;
		}
		void SetFileOffset(u32 offset)
		{
			m_FileOffset = offset >> FileOffset_Shift;
		}
		int GetVersion() const
		{
			return IsResource() ? (int)u.resource.m_Info.GetVersion() : 0;
		}

		static constexpr u64 FileOffset_IsDir = (1 << 23) - 1;
		static constexpr int FileOffset_Shift = 9;
		static constexpr u32 MaxConsumedSize = 0xFFFFFF;
		u64 m_NameOffset : 16,
			m_ConsumedSize : 24,
			m_FileOffset : 23,
			m_IsResource : 1;
		union
		{
			struct
			{
				u32 m_UncompressedSize; // For files; file is compressed if m_ConsumedSize is nonzero.  Resources are always compressed.
				u32 m_Encrypted; // Encryption key ID, or 0 if not encrypted. Only compressed files will be encrypted for now?
			} file;
			struct
			{
				u32 m_DirectoryIndex; // For directories; first index in pack array of the directory
				u32 m_DirectoryCount; // For directories; count of entries in the directory.
			} directory;
			struct
			{
				datResourceInfo m_Info;
			} resource;
		} u;
	};
	class fiCollection : public fiDevice
	{
	public:
		fiCollection() : m_isStreaming(true)
		{}
		virtual void Shutdown()
		{}
		virtual fiHandle OpenBulkFromHandle(u32 handle, u64& offset) const = 0;
		virtual const fiPackEntry& GetEntryInfo(u32 handle) const = 0;
		virtual u32 GetEntryPhysicalSortKey(u32 handle, bool uncached) const = 0;
		virtual const char* GetEntryName(u32 handle) const = 0;
		virtual const char* GetEntryFullName(u32 handle, char* dest, int destSize) const = 0;		// Slower
		virtual int GetEntryIndex(const char* name) const = 0;
		virtual u32 GetBasePhysicalSortKey() const = 0;
		virtual bool Prefetch(u32 handle) const = 0;
		virtual bool IsPackfile() const
		{
			return false;
		}
		virtual bool IsStreaming() const
		{
			return m_isStreaming;
		}
		virtual void SetStreaming(bool val)
		{
			m_isStreaming = val;
		}

		bool m_isStreaming;
		static constexpr u32 EntryMask = 0xFFFF;
		static constexpr int ArchiveShift = 16;
		static constexpr u32 ArchiveMask = 0xFFFF;
	};
	class fiPackfile : public fiCollection
	{
	public:
		virtual ~fiPackfile()
		{}
		virtual void Shutdown()
		{}
		virtual const fiPackEntry* FindEntry(const char* name) const
		{
			return nullptr;
		}
		virtual fiHandle Open(const char* filename, bool readOnly) const
		{
			return nullptr;
		}
		virtual fiHandle OpenDirect(const char* filename, bool readOnly) const
		{
			return nullptr;
		}
		virtual fiHandle Create(const char* filename) const
		{
			return nullptr;
		}
		virtual int Read(fiHandle handle, void* outBuffer, int bufferSize) const
		{
			return NULL;
		}
		virtual int Write(fiHandle handle, const void* buffer, int bufferSize) const
		{
			return NULL;
		}
		virtual int Seek(fiHandle handle, int offset, fiSeekWhence whence) const
		{
			return NULL;
		}
		virtual u64 Seek64(fiHandle handle, s64 offset, fiSeekWhence whence) const
		{
			return NULL;
		}
		virtual int Size(fiHandle handle) const
		{
			return NULL;
		}
		virtual u64 Size64(fiHandle handle) const
		{
			return NULL;
		}
		virtual int Close(fiHandle handle) const
		{
			return NULL;
		}
		virtual u64 GetFileSize(const char* filename) const
		{
			return NULL;
		}
		virtual u64 GetFileTime(const char* filename) const
		{
			return NULL;
		}
		virtual bool SetFileTime(const char* filename, u64 timestamp) const
		{
			return false;
		}
		virtual u32 GetAttributes(const char* filename) const
		{
			return NULL;
		}
		virtual fiHandle OpenBulk(const char* filename, u64& outBias) const
		{
			return nullptr;
		}
		virtual int ReadBulk(fiHandle handle, u64 offset, void* outBuffer, int bufferSize) const
		{
			return NULL;
		}
		virtual int CloseBulk(fiHandle handle) const
		{
			return NULL;
		}
		virtual fiHandle FindFileBegin(const char* directoryName, fiFindData& outData)
		{
			return NULL;
		}
		virtual bool FindFileNext(fiHandle handle, fiFindData& outData)
		{
			return NULL;
		}
		virtual int FindFileEnd(fiHandle handle)
		{
			return NULL;
		}
		virtual int GetResourceInfo(const char* name, datResourceInfo& outHeader)
		{
			return NULL;
		}
		virtual u64 GetBulkOffset(const char* name)
		{
			return NULL;
		}
		virtual u32 GetPhysicalSortKey(const char* pName)
		{
			return NULL;
		}
		virtual u32 GetPackfileIndex() const
		{
			return m_SelfIndex;
		}
		virtual u32 GetEncryptionKey() const
		{
			return m_KeyId;
		}
		virtual const fiPackEntry& GetEntryInfo(u32 handle) = 0;
		virtual fiHandle OpenBulkFromHandle(u32 handle, u64& offset)
		{
			return NULL;
		}
		virtual u32 GetEntryPhysicalSortKey(u32 handle, bool)
		{
			return NULL;
		}
		virtual const char* GetEntryName(u32 handle)
		{
			return "";
		}
		virtual const char* GetEntryFullName(u32 handle, char* dest, int destSize)
		{
			return "";
		}
		virtual u32 GetBasePhysicalSortKey()
		{
			return NULL;
		}
		virtual int GetEntryIndex(const char* name)
		{
			return NULL;
		}
		virtual bool IsRpf()
		{
			return false;
		}
		virtual const char* GetDebugName()
		{
			return "";
		}
		virtual bool Prefetch(u32) const
		{
			return true;
		}
		virtual bool IsPackfile() const
		{
			return true;
		}
		virtual RootDeviceId GetRootDeviceId(const char* /*name*/) const
		{
			if (m_ForceOnOdd)
				return OPTICAL;
			return m_Device->GetRootDeviceId(m_Fullname.c_str());
		}
		typedef u16 ParentIndexType;
		char* m_NameHeap; // Nameheap for all filenames in the zipfile.
		ParentIndexType* m_ParentIndices;
		fiPackEntry* m_Entries; // Master entry table; entry 0 is the root directory
		u32 m_EntryCount;
		fiHandle m_Handle; // (bulk) handle on the device of the zipfile itself
		u64 m_FileTime; // File time of the zipfile itself
		u64 m_ArchiveBias; // For nested zipfiles (sigh)
		u64 m_ArchiveSize;
		const fiDevice* m_Device; // Device the zipfile itself lives on
		int m_RelativeOffset; // Amount of incoming pathname to strip off
		char m_Name[32]; // Debug name
		atString m_Fullname;
		bool m_IsXCompressed;
		bool m_IsByteSwapped;
		u16 m_SelfIndex;
		u32 m_SortKey; // sort key used to define position on a DVD
		const void* m_pDrmKey; // Drmkey used to decode drm content (ps3 only?)
		char* m_MemoryBuffer; // Buffer to put packfile into if caching in memory
		bool m_IsRpf;
		bool m_IsCached;
		bool m_IsInstalled;
		bool m_IsUserHeader; // If true, the user provided the memory for the header data and is responsible for its clean-up.
		u32 m_CachedMetaDataSize; // Cached meta data size, only valid after having called Init/ReInit() at least once.
		u32 m_CachedDataSize; // Cached size for the header (including both the initial header as well as all entries).
		u32 m_KeyId; // Encryption key ID
		u8 m_NameShift;
		bool m_ForceOnOdd;
		bool m_KeepNameHeap; // Should we persist the name heap? i.e., don't delete on UnInit()/Shutdown(), etc.
	};
}
//ioXXXXXXX
namespace rage
{
	struct ioPad
	{
		enum
		{
			MAX_PADS = 4
		};
	};
}
//netXXXXXX
namespace rage
{
	#define DEFINE_AT_RTTI(T) private: \
		virtual T* GetIdentifier() { return nullptr; } \
		virtual T* GetIdentifier_2() { return nullptr; } \
		virtual uint32_t GetTypeHash() { return NULL; } \
		virtual T* GetIfIsOfType(T* vft) { return vft; } \
		virtual void IsOfType(T* vft) {} \
		virtual void IsOfTypeObject(T* object) {} \
		public: \
		virtual void Deconstructor() {}

	class netStatus
	{
	public:
		enum StatusCode
		{
			NET_STATUS_NONE,
			NET_STATUS_PENDING,
			NET_STATUS_FAILED,
			NET_STATUS_SUCCEEDED,
			NET_STATUS_CANCELED,
			NET_STATUS_COUNT
		};
		union
		{
			u32 m_Status;
			StatusCode m_StatusCode;
		};
		int m_ResultCode;
	};
	typedef u16 netSequence;
	class netLoggingInterface
	{
	public:
		enum LoggingLevel
		{
			LOG_LEVEL_LOW, // Low importance messages
			LOG_LEVEL_MEDIUM, // Medium importance messages
			LOG_LEVEL_HIGH, // High importance messages
			LOG_LEVEL_ERROR, // Error messages
		};
		virtual void Log(const char* logText, ...) = 0;
		virtual void LineBreak() = 0;
		virtual void Flush(bool waitForFlushToComplete = false) = 0;
		virtual void Disable() = 0;
		virtual void Enable() = 0;
		virtual bool IsEnabled() const = 0;
		virtual void WriteDataValue(const char* dataName, const char* dataValue, ...) = 0;
		virtual void WriteMessageHeader(bool received, netSequence sequence, const char* messageString1, const char* messageString2, const char* playerName) = 0;
		virtual void WritePlayerText(const char* dataName, const char* dataValue, const char* playerName) = 0;
		virtual void WriteEventData(const char* eventName, const char* eventData, unsigned trailingLineBreaks) = 0;
		virtual void WriteNodeHeader(const char* nodeName) = 0;
		virtual void PostNetworkUpdate()
		{}
		virtual void GetDebugStatisticsText(char* buffer, const unsigned bufferSize)
		{}
		virtual void GetFileName(char* buffer, u32 bufferSize) const
		{}
	protected:
		virtual ~netLoggingInterface()
		{}
	};
	class rlGamerInfo;
	class netPlayer
	{
		friend class netPlayerMgrBase;
	public:
		enum PlayerType
		{
			NETWORK_PLAYER,
			NETWORK_BOT,
			MAX_PLAYER_TYPES
		};
		static const int VOICE_CHANNEL_NONE = -1;
		DEFINE_AT_RTTI(netPlayer)
		virtual void Shutdown() = 0;
		virtual bool IsPhysical() const = 0;
		virtual const char* GetLogName() const = 0;
		virtual void ActiveUpdate() = 0;
		virtual void Unk58() = 0;
		virtual bool IsHost() const = 0;
		virtual const rlGamerInfo* GetGamerInfo() const = 0;
	};
	class netRelayToken
	{
	public:
		static const unsigned MAX_TOKEN_BUF_SIZE = 6;
		static const unsigned MAX_EXPORTED_SIZE_IN_BYTES = MAX_TOKEN_BUF_SIZE;

		// 2 hex chars per byte + 2 for "0x", + 1 for null terminator
		static const unsigned TO_HEX_STRING_BUFFER_SIZE = (MAX_TOKEN_BUF_SIZE * 2) + 2 + 1;

		static const netRelayToken INVALID_RELAY_TOKEN;
		u8 m_Token[MAX_TOKEN_BUF_SIZE];
	};
	enum netNatType
	{
		NET_NAT_UNKNOWN,
		NET_NAT_OPEN,
		NET_NAT_MODERATE,
		NET_NAT_STRICT,
		NET_NAT_NUM_TYPES
	};
	class netPeerId
	{
	public:
		static const u64 NET_INVALID_PEER_ID = ~u64(0);
		static const unsigned MAX_EXPORTED_SIZE_IN_BYTES = sizeof(u64);
		static const unsigned TO_HEX_STRING_BUFFER_SIZE = 19;
		u64 m_Id;
	};
	typedef s32 PlayerAccountId;
	typedef s64 RockstarId;
	class rlGamerHandle
	{
	public:
		static const unsigned MAX_EXPORTED_SIZE_IN_BITS_SC =	(sizeof(u8) << 3) +	// m_Service
			(sizeof(RockstarId) << 3) +	// m_RockstarId
			(sizeof(u8) << 3); // m_BotIndex
		static const unsigned MAX_EXPORTED_SIZE_IN_BYTES_SC = ((MAX_EXPORTED_SIZE_IN_BITS_SC + 7) & ~7) >> 3; // round up to nearest multiple of 8 then divide by 8
		union Data
		{
			struct ScData
			{
				RockstarId m_RockstarId;
			} m_ScData;
		} m_Data;
		u8 m_Service;
		#define INVALID_BOT_INDEX (0)
		#define MAX_BOT_INDEX (31)
		u8 m_BotIndex;
	};
	class netCrypto
	{
	public:
		class DiffieHellman
		{
		public:
			static const unsigned MAX_PRIME_SIZE_IN_BITS = 1024;
			static const unsigned MAX_PRIME_SIZE_IN_BYTES = MAX_PRIME_SIZE_IN_BITS >> 3;
			static const unsigned MAX_GENERATOR_SIZE_IN_BYTES = MAX_PRIME_SIZE_IN_BYTES;
			static const unsigned MAX_PUBLIC_KEY_LENGTH_IN_BYTES = MAX_PRIME_SIZE_IN_BYTES;
			static const unsigned MAX_PRIVATE_KEY_LENGTH_IN_BYTES = MAX_PUBLIC_KEY_LENGTH_IN_BYTES;
			static const unsigned MAX_AGREE_KEY_LENGTH_IN_BYTES = MAX_PUBLIC_KEY_LENGTH_IN_BYTES;
			struct Impl;
			Impl* m_pImpl;
			bool m_Initialized;
			bool m_GeneratedKeyPair;
		};
	};
	class netP2pCrypt
	{
	public:
		static const unsigned MAX_SIZEOF_OVERHEAD_IN_BYTES = (1 + 4 + 8 + 16);
		class Key
		{
		public:
			static const unsigned KEY_LENGTH_IN_BITS = 256;
			static const unsigned KEY_LENGTH_IN_BYTES = KEY_LENGTH_IN_BITS >> 3;
			static const unsigned MAX_EXPORTED_SIZE_IN_BYTES = KEY_LENGTH_IN_BYTES;
			u8 m_RawKey[KEY_LENGTH_IN_BYTES];
			bool m_IsValid;
		};
		class PublicKey
		{
		public:
			static const unsigned KEY_LENGTH_IN_BYTES = netCrypto::DiffieHellman::MAX_PUBLIC_KEY_LENGTH_IN_BYTES;
			static const unsigned KEY_LENGTH_IN_BITS = KEY_LENGTH_IN_BYTES << 3;
			static const unsigned MAX_EXPORTED_SIZE_IN_BITS = datBitsNeeded<KEY_LENGTH_IN_BYTES>::COUNT + KEY_LENGTH_IN_BITS;
			static const unsigned MAX_EXPORTED_SIZE_IN_BYTES = ((MAX_EXPORTED_SIZE_IN_BITS + 7) & ~7) >> 3; // round up to nearest multiple of 8 then divide by 8
			u8 m_RawKey[KEY_LENGTH_IN_BYTES];
			unsigned m_KeyLengthInBytes;
			bool m_IsValid;
		};
		static const unsigned HMAC_KEY_MAX_LEN = 32;
		static const unsigned HMAC_SALT_MAX_LEN = 32;
		typedef u8 HmacKeySalt[HMAC_KEY_MAX_LEN + HMAC_SALT_MAX_LEN];
		static const unsigned P2P_CRYPT_HMAC_LENGTH_IN_BYTES = (256 >> 3); // 256-bit hash
		static const unsigned P2P_CRYPT_TRUNCATED_HMAC_LENGTH_IN_BYTES = (64 >> 3); // 64-bit truncated hash
	};
	enum ConnectReason
	{
		CR_INVALID,
		CR_ELASTIC,
		CR_CDN_UPLOAD_PHOTO,
		CR_CDN_UPLOAD_VIDEO,
		CR_ORBIS_MEDIA_DECODER,
		CR_CLOUD,
		CR_TELEMETRY_UNUSED,
		CR_STATS_UNUSED,
		CR_ENTITLEMENT,
		CR_FACEBOOK,
		CR_SC_CREATE_AUTH_TOKEN,
		CR_SC_CREATE_LINK_TOKEN,
		CR_ROS_CREATE_TICKET,
		CR_ROS_CREDENTIALS_CHANGING,
		CR_ROS_GET_GEOLOCATION_INFO,
		CR_ROS_GET_NAT_DISCOVERY_SERVERS,
		CR_SOCIAL_CLUB,
		CR_UGC,
		CR_CLOUD_SAVE,
		CR_ACHIEVEMENTS,
		CR_MATCHMAKING,
		CR_FRIENDS,
		CR_CREW,
		CR_CONDUCTOR_ROUTE,
		CR_PRESENCE,
		CR_INBOX,
		CR_YOUTUBE,
		CR_SAVE_MIGRATION,
		CR_FEED,
		CR_REGISTER_PUSH_NOTIFICATION_DEVICE,
		CR_GAME_SERVER,
		CR_ITEM_DATABASE,
		CR_POSSE,
		CR_TEST,
		CR_BUGSTAR,
		CR_GOOGLE_ANALYTICS,
		CR_DAILY_OBJECTIVES,
		CR_SP_ACTION_PROXY,
		CR_SESSION_SERVER,
		CR_CASH_INVENTORY_SERVER,
		CR_BOUNTY_SERVER,
		CR_ANTI_CHEAT_SERVER,
		CR_MINIGAME_SERVER,
		CR_MESSAGE_DISTRIBUTION_SERVER,
		CR_COUNT
	};
	enum netHttpAbortReason
	{
		NET_HTTPABORT_NONE = 0,
		NET_HTTPABORT_COMMIT_FAILED = 1,
		NET_HTTPABORT_CANCELED = 2,
		NET_HTTPABORT_REQUEST_TIMED_OUT = 3,
		NET_HTTPABORT_RESOLVE_HOST_FAILED = 4,
		NET_HTTPABORT_RESOLVING_HOST_FAILED = 5,
		NET_HTTPABORT_CONNECT_HOST_FAILED = 6,
		NET_HTTPABORT_CONNECTING_HOST_FAILED = 7,
		NET_HTTPABORT_SEND_HEADER_FAILED = 8,
		NET_HTTPABORT_RECEIVE_HEADER_FAILED = 9,
		NET_HTTPABORT_PROCESS_RESPONSE_HEADER_FAILED = 10,
		NET_HTTPABORT_RESPONSE_NO_LOCATION_LENGTH = 11,
		NET_HTTPABORT_RESPONSE_REDIRECT_FAILED = 12,
		NET_HTTPABORT_RESPONSE_GET_LOCATION_FAILED = 13,
		NET_HTTPABORT_RESPONSE_UNHANDLED_TRANSFER_ENCODING = 14,
		NET_HTTPABORT_RESPONSE_PARSE_CONTENT_LENGTH_FAILED = 15,
		NET_HTTPABORT_READ_BODY_FAILED = 16,
		NET_HTTPABORT_CONSUME_BODY_FAILED = 17,
		NET_HTTPABORT_CHUNK_OPERATION_FAILED = 18,
		NET_HTTPABORT_ALLOCATE_CHUNK_FAILED = 19,
		NET_HTTPABORT_SEND_CHUNK_FAILED = 20,
		NET_HTTPABORT_XBL_RECEIVE_HEADER_FAILED = 21,
		NET_HTTPABORT_XBL_RECEIVE_BODY_FAILED = 22,
		NET_HTTPABORT_PROCESS_BOUNCE_CONTENT_FAILED = 23,
		NET_HTTPABORT_READ_BODY_ONCE_FAILED = 24,
		NET_HTTPABORT_XBOX_AUTH_TOKEN_FAILED = 25,
	};
	class netIpV4Address
	{
	public:
		union
		{
			u32 pck;
			struct
			{
				u8 f4, f3, f2, f1;
			};
		} m_Ip;
	};
	class netIpAddress
	{
	public:
		netIpV4Address m_IpAddress;
	};
	class netSocketAddress
	{
	public:
		netIpAddress m_Ip;
		u16 m_Port;
	};
	class netTcpAsyncOp
	{
		friend class netTcp;
		friend class netHttpRequest;
	public:
		enum OpType
		{
			OP_INVALID = -1,
			OP_CONNECT,
			OP_SSL_CONNECT,
			OP_RECV_BUFFER,
			OP_SEND_BUFFER,
			OP_NUM_TYPES
		};
		OpType m_Type;
		int m_SktId;
		int m_LastError;
		enum ConnectState
		{
			STATE_SOCKET_CONNECT,
			STATE_SSL_CONNECT
		};
		enum SslSocketOp
		{
			SSL_SOCKET_OP_READ,
			SSL_SOCKET_OP_WRITE
		};
		union
		{
			struct
			{
				u8 m_AddrBuf[sizeof(netSocketAddress)];
				netSocketAddress* m_Addr;
				int* m_CallerSktId;
				ConnectReason m_ConnectReason;
				ConnectState m_State;
			} m_Connect;
			struct
			{
				u8 m_AddrBuf[sizeof(netSocketAddress)];
				netSocketAddress* m_Addr;
				int* m_CallerSktId;
				ConnectState m_State;
				SslSocketOp m_NextSocketOp;
				SSL_CTX* m_SslCtx;
				const char* m_DomainName;
				ConnectReason m_ConnectReason;
			} m_SslConnect;
			struct
			{
				void* m_Buf;
				u32 m_SizeofBuf;
				u32 m_NumRcvd;
			} m_RcvBuf;
			struct
			{
				const void* m_Buf;
				u32 m_SizeofBuf;
				u32 m_NumSent;
			} m_SndBuf;
		} m_Data;
		int m_TimeoutMs;
		netStatus m_MyStatus;
		u32 m_Id;
		inlist_node<netTcpAsyncOp> m_ListLink;
	};
	enum netHttpVerb
	{
		NET_HTTP_VERB_INVALID,
		NET_HTTP_VERB_GET,
		NET_HTTP_VERB_POST,
		NET_HTTP_VERB_PUT,
		NET_HTTP_VERB_DELETE,
		NET_HTTP_VERB_HEAD,
		NET_HTTP_VERBS_COUNT
	};
	class netTimeout
	{
	public:
		static constexpr u32 DEFAULT_LONG_FRAME_THRESHOLD_MS = (3 * 1000);
		u32 m_CurTime;
		u32 m_LongFrameThresholdMs;
		int m_Timeout;
		int m_Countdown;
	};
	enum netTcpResult
	{
		NETTCP_RESULT_DISCONNECTED = -4,
		NETTCP_RESULT_TIMED_OUT,
		NETTCP_RESULT_ERROR,
		NETTCP_RESULT_CANCELED,
		NETTCP_RESULT_OK
	};
	class netHttpRequest
	{
	public:
		static constexpr u32 DEFAULT_BOUNCE_BUFFER_MAX_LENGTH = 1044;
		enum TransferEncoding
		{
			TRANSFER_ENCODING_NORMAL,
			TRANSFER_ENCODING_CHUNKED,
		};
		enum UriScheme
		{
			URISCHEME_INVALID,
			URISCHEME_HTTP,
			URISCHEME_HTTPS,
		};
		struct Chunk
		{
			datGrowBuffer m_Data;
			netTcpAsyncOp m_TcpOp;
			netStatus m_Status; // when not using m_TcpOp, such as when IXHR2 is used.
			inlist_node<Chunk> m_ListLink;
		};
		enum SendState
		{
			SENDSTATE_NONE,
			SENDSTATE_WAIT_UNTIL_READY,
			SENDSTATE_RESOLVE_HOST,
			SENDSTATE_RESOLVING_HOST,
			SENDSTATE_CONNECT,
			SENDSTATE_CONNECTING,
			SENDSTATE_SEND_HEADER,
			SENDSTATE_SENDING_CONTENT,
			SENDSTATE_SENDING_TERM_CHUNK,
			SENDSTATE_SUCCEEDED,
			SENDSTATE_ERROR
		};
		enum RecvState
		{
			RECVSTATE_NONE,
			RECVSTATE_RECEIVING_HEADER,
			RECVSTATE_RECEIVED_HEADER,
			RECVSTATE_RECEIVING_NORMAL_CONTENT,
			RECVSTATE_RECEIVING_CHUNKED_CONTENT,
			RECVSTATE_FINISH_CONSUMING_CONTENT,
			RECVSTATE_SUCCEEDED,
			RECVSTATE_ERROR
		};
		Chunk* m_CurChunk;
		typedef inlist<Chunk, &Chunk::m_ListLink> ChunkList;
		ChunkList m_QueuedChunks;
		Chunk* m_InFlightChunk;
		datGrowBuffer m_InBuf;
		atStringBuilder m_InHeader;
		atStringBuilder m_OutHeader;
		class netHttpFilter* m_Filter;
		fiHandle m_ResponseHandle;
		const fiDevice* m_ResponseDevice;
		char m_DefaultBounceBuf[DEFAULT_BOUNCE_BUFFER_MAX_LENGTH];
		char* m_BounceBuf;
		u32 m_BounceBufMaxLen;
		u32 m_BounceBufLen;
		u32 m_RequestId;
		netHttpVerb m_HttpVerb;
		mutable int m_HttpStatusCode;
		UriScheme m_UriScheme;
		char* m_RawUri;
		const char* m_RawHost;
		const char* m_RawPath;
		mutable char* m_EncodedUri;
		atStringBuilder m_QueryString;
		atStringBuilder m_UserAgentString;
		u32 m_InContentLen;
		u32 m_InContentBytesRcvd;
		u32 m_OutContentBytesSent;
		u32 m_OutChunksBytesSent;
		netSocketAddress m_ProxyAddr;
		bool m_IgnoreProxy;
		netTimeout m_Timeout;
		int m_RedirectCount;
		int m_Skt;
		SendState m_SendState;
		RecvState m_RecvState;
		sysMemAllocator* m_Allocator;
		u32 m_Options;
		u32 m_ByteRangeLow;
		u32 m_ByteRangeHigh;
		u32 m_NumFormParams;
		u32 m_NumQueryParams;
		int m_NumListParamValues;
		netIpAddress m_HostIp;
		u16 m_HostPort;
		netTcpAsyncOp m_ConnectTcpOp;
		// Diagnostics
		netHttpAbortReason m_AbortReason;
		bool m_ChunkAllocationError;
		netTcpResult m_TcpOpResult;
		int m_TcpOpError;
		TransferEncoding m_OutboundTransferEncoding;
		netStatus* m_Status;
		netStatus m_MyStatus;
		netStatus m_HostResolutionStatus;
		//For managing a linked list of requests.
		netHttpRequest* m_Next;
		netHttpRequest* m_Prev;
		WOLFSSL_CTX* m_SslCtx;
	};
	class netHttpFilter
	{
		friend class netHttpRequest;
	public:
		virtual ~netHttpFilter()
		{}
		virtual bool CanFilterRequest() const
		{
			return false;
		}
		virtual bool CanFilterResponse() const
		{
			return false;
		}
		virtual u32 DelayMs() const
		{
			return 0;
		}
		virtual bool CanIntercept() const
		{
			return false;
		}
		virtual bool FilterRequest(const u8* data, const u32 dataLen, const bool finalCall, datGrowBuffer& output)
		{
			return false;
		}
		virtual bool FilterResponse(const u8* data, const u32 dataLen, const bool allDataReceived, fiHandle& outputHandle, const fiDevice* outputDevice, u32* numProcessed, bool* hasDataPending)
		{
			return false;
		}
		virtual const char* GetUserAgentString() const
		{
			return NULL;
		}
		virtual bool ProcessRequestHeader(class netHttpRequest* request)
		{
			return false;
		}
		virtual bool ProcessResponseHeader(const int statusCode, const char* header)
		{
			return false;
		}
		virtual void Receive(void* buf, const u32 bufSize, u32* bytesReceived)
		{}
		virtual bool AllowSucceeded(class netHttpRequest* request)
		{
			return false;
		}
		virtual void DisableHttpDump(bool /*bDisabled*/)
		{}
		const netHttpRequest* m_HttpRequest;
	};
	class netPeerAddress
	{
	public:
		netPeerId m_PeerId;
		rlGamerHandle m_GamerHandle;
		netP2pCrypt::Key m_Key;
		netSocketAddress m_RelayServerAddress;
		netRelayToken m_RelayToken;
		netSocketAddress m_PublicAddress;
		netSocketAddress m_PrivateAddress;
		netNatType m_NatType;
	};
	struct ReceivedMessageData
	{
		ReceivedMessageData(const void* messageData, u32 messageDataSize, netSequence sequence, netPlayer* fromPlayer, netPlayer* toPlayer) :
			m_MessageData(messageData),
			m_MessageDataSize(messageDataSize),
			m_FromPlayer(fromPlayer),
			m_ToPlayer(toPlayer),
			m_NetSequence(sequence)
		{}

		bool IsValid() const
		{
			return m_MessageData && m_FromPlayer && m_ToPlayer;
		}

		const void* m_MessageData;
		u32 m_MessageDataSize;
		netPlayer* m_FromPlayer;
		netPlayer* m_ToPlayer;
		netSequence m_NetSequence;
	};
	enum netMessageId
	{
		CMSG_TEXT_CHAT_STATUS = 0,
		REASSIGN_NEGOTIATE_MSG = 1,
		MSG_TRANSITION_TO_GAME_NOTIFY = 2,
		CMSG_VOICE_STATUS = 3,
		MSG_RESERVE_SLOTS_ACK = 4,
		RL_MSG_SEARCH_REQUEST = 5,
		MSG_TRANSITION_TO_ACTIVITY_START = 6,
		MSG_SESSION_ESTABLISHED = 7,
		CMSG_JOIN_RESPONSE = 8,
		INFORM_OBJECT_IDS_MSG = 9,
		MSG_TEXT_MESSAGE = 10,
		MSG_SCRIPT_VERIFY_HOST_ACK = 11,
		MSG_BLACKLIST = 12,
		MSG_REQUEST_KICK_FROM_HOST = 13,
		MSG_SCRIPT_NEW_HOST = 14,
		NET_KEY_EXCHANGE_SESSION_OFFER = 15,
		REASSIGN_RESPONSE_MSG = 16,
		NET_ICE_SESSION_PONG = 17,
		SN_MSG_MIGRATE_HOST_RESPONSE = 18,
		RL_SESSION_DETAIL_RESPONSE = 19,
		MSG_TRANSITION_GAMER_INSTRUCTION = 20,
		NET_ICE_SESSION_PING = 21,
		NON_PHYSICAL_DATA_MSG = 22,
		MSG_SCRIPT_LEAVE = 23,
		PLAYER_DATA_MSG = 24,
		MSG_TRANSITION_LAUNCH = 25,
		MSG_SCRIPT_MIGRATE_HOST_FAIL_ACK = 26,
		MSG_TRANSITION_LAUNCH_NOTIFY = 27,
		MSG_SCRIPT_BOT_JOIN = 28,
		RL_MSG_QOS_PROBE_REQUEST = 29,
		MSG_PARTY_ENTER_GAME = 30,
		SN_MSG_SET_INVITABLE_CMD = 31,
		CMSG_PACKED_EVENT_RELIABLES_MSGS = 32,
		NET_MESSAGE = 33,
		RL_SESSION_DETAIL_REQUEST = 34,
		SN_MSG_SESSION_MEMBER_IDS = 35,
		CMSG_TEXT_MESSAGE = 36,
		MSG_SPLIT_UPDATE_ACK = 37,
		REASSIGN_CONFIRM_MSG = 38,
		NET_LAG_PING_MSG = 39,
		NET_ICE_SESSION_DEBUG_INFO = 40,
		REQUEST_OBJECT_IDS_MSG = 41,
		SN_MSG_JOIN_RESPONSE = 42,
		MSG_SCRIPT_BOT_LEAVE = 43,
		RL_MSG_QOS_PROBE_RESPONSE = 44,
		SN_MSG_START_MATCH_CMD = 45,
		CROAMING_REQUEST_BUBBLE_MSG = 46,
		MSG_CXN_REQUEST_REMOTE_TIMEOUT = 47,
		PACKED_RELIABLES_MSG = 48,
		MSG_SCRIPT_BOT_HANDSHAKE_ACK = 49,
		CROAMING_INITIAL_BUBBLE_MSG = 50,
		MSG_SCRIPT_MIGRATE_HOST = 51,
		MSG_KICK_PLAYER = 52,
		SN_MSG_NOT_MIGRATING = 53,
		MSG_TRANSITION_TO_ACTIVITY_FINISH = 54,
		MSG_TRANSITION_PARAMETER_STRING = 55,
		NET_TIME_SYNC_MSG = 56,
		MSG_RESERVE_SLOTS = 57,
		MSG_PLAYER_CARD_SYNC = 58,
		PACKED_CLONE_SYNC_ACKS_MSG = 59,
		MSG_TRANSITION_PARAMETERS = 60,
		MSG_PARTY_LEAVE_GAME = 61,
		MSG_SCRIPT_VERIFY_HOST = 62,
		CROAMING_JOIN_BUBBLE_ACK_MSG = 63,
		MSG_SCRIPT_LEAVE_ACK = 64,
		SN_MSG_JOIN_REQUEST = 65,
		MSG_REQUEST_TRANSITION_PARAMETERS = 66,
		MSG_SCRIPT_JOIN_ACK = 67,
		NET_KEY_EXCHANGE_SESSION_ANSWER = 68,
		NET_ICE_SESSION_PORT_OPENER = 69,
		MSG_RADIO_STATION_SYNC = 70,
		MSG_RADIO_STATION_SYNC_REQUEST = 71,
		SN_MSG_CONFIG_REQUEST = 72,
		MSG_CXN_RELAY_ADDR_CHANGED = 73,
		NET_ICE_SESSION_ANSWER_OLD = 74, // unversioned struct - no longer used
		MSG_SCRIPT_BOT_HANDSHAKE = 75,
		CROAMING_JOIN_BUBBLE_MSG = 76,
		MSG_SCRIPT_JOIN_HOST_ACK = 77,
		CLONE_SYNC_MSG = 78,
		CMSG_PACKED_EVENTS = 79,
		MSG_TRANSITION_TO_GAME_START = 80,
		MSG_CHECK_QUEUED_JOIN_REQUEST = 81,
		MSG_SESSION_ESTABLISHED_REQUEST = 82,
		SN_MSG_REMOVE_GAMERS_FROM_SESSION_CMD = 83,
		SN_MSG_REQUEST_GAMER_INFO = 84,
		NET_COMPLAINT_MSG = 85,
		NET_ICE_SESSION_OFFER_OLD = 86,	// unversioned struct - no longer used
		MSG_SCRIPT_HANDSHAKE = 87,
		SN_MSG_HOST_LEFT_WHILST_JOINING_CMD = 88,
		MSG_CHECK_QUEUED_JOIN_REQUEST_REPLY = 89,
		SN_MSG_CHANGE_SESSION_ATTRIBUTES_CMD = 90,
		MSG_SCRIPT_HANDSHAKE_ACK = 91,
		MSG_SCRIPT_JOIN = 92,
		MSG_UPDATE_ACK = 93,
		MSG_CHECK_QUEUED_JOIN_REQUEST_INVITE_REPLY = 94,
		SN_MSG_CONFIG_RESPONSE = 95,
		MSG_UPDATE = 96,
		CMSG_PLAYER_IS_TYPING = 97,
		SN_MSG_SESSION_ACCEPT_CHAT = 98,
		MSG_SCRIPT_BOT_JOIN_ACK = 99,
		SN_MSG_ADD_GAMER_TO_SESSION_CMD = 100,
		ACTIVATE_NETWORK_BOT_MSG = 101,
		SN_MSG_MIGRATE_HOST_REQUEST = 102,
		MSG_SCRIPT_HOST_REQUEST = 103,
		CMSG_JOIN_REQUEST = 104,
		SN_MSG_BASE = 105,
		MSG_PLAYER_CARD_REQUEST = 106,
		RL_MSG_SEARCH_RESPONSE = 107,
		MSG_DEBUG_TIME_SCALE = 108,
		MSG_DEBUG_TIME_PAUSE = 109,
		MSG_DEBUG_NO_TIMEOUTS = 110,
		MSG_DEBUG_ADD_FAIL_MARK = 111,
		MSG_SYNC_DR_DISPLAY = 112,
		SN_MSG_SESSION_INFO = 113,
		SN_MSG_SESSION_INFO_RESPONSE = 114,
		SN_MSG_SESSION_JOIN_REQUEST = 115,
		SN_MSG_SESSION_JOIN_REQUEST_RESPONSE = 116,
		CMSG_VOICE_CHAT_REQUEST = 117,
		CMSG_VOICE_CHAT_REPLY = 118,
		CMSG_VOICE_CHAT_END = 119,
		VOICE_CHAT_USER_MSG = 120,
		SESSION_INFO_MSG = 121,
		REGISTER_COMPLETE_MSG = 122,
		START_COMMAND_MSG = 123,
		QA_NR_FOO_MSG = 124,
		QA_NR_BAR_MSG = 125,
		MSG_DEBUG_STALL = 126,
		NET_ICE_SESSION_OFFER = 127,
		NET_ICE_SESSION_ANSWER = 128,
		MSG_LOST_HOST_CONNECTION = 129,
		CROAMING_BUBBLE_REQUIRED_CHECK_MSG = 130,
		CROAMING_BUBBLE_REQUIRED_RESPONSE_MSG = 131,
		NET_ROUTE_REQUEST_MSG = 132,
		NET_ROUTE_REPLY_MSG = 133,
		NET_ROUTE_ERR_MSG = 134,
		NET_ROUTE_CHANGE_REQUEST_MSG = 135,
		NET_ROUTE_CHANGE_REPLY_MSG = 136,
		NET_ICE_SESSION_RELAY_ROUTE_CHECK = 137,
		// add new message ids before this
		MSG_ID_NUM_MESSAGE_IDS
	};
}
//parXXXXXX
namespace rage
{
	class parAttribute
	{
	public:
		enum Flags
		{
			FLAG_OWNS_NAME_STR,
			FLAG_OWNS_VALUE_STR,
			FLAG_HIGH_PRECISION,
			FLAG_UNSIGNED,
			FLAG_HEXADECIMAL,
		};
		const char* m_Name;
		union
		{
			const char* m_String;
			s64 m_Int64;
			double m_Double;
			bool m_Bool;
		} m_Value;
		u8 m_Type;
		atFixedBitSet8 m_Flags;
	};
	class parAttributeList
	{
	public:
		atArray<parAttribute> m_Attributes;
		u8 m_UserData1, m_UserData2;
		atFixedBitSet16 m_Flags;
	};
	class parElement
	{
	public:
		const char* m_Name;
		parAttributeList m_Attributes;
	};
	class parTreeNode
	{
	public:
	protected:
		friend class parTree;
		parElement m_Element;
		parTreeNode* m_Parent;
		parTreeNode* m_Sibling;
		parTreeNode* m_Child;
		atArray<char, u32> m_DataArray;
	};
	typedef atArray<char*> parNameTable;
	class parTree
	{
	public:
		parTreeNode* m_Root;
		parNameTable m_NameTable;
		parTreeNode* m_Current;
		parTreeNode* m_Sibling;
	};
}
//rlXXXXXXX
namespace rage {
	typedef netPeerAddress rlPeerAddress;
	class rlPeerInfo
	{
	public:
		rlPeerAddress m_PeerAddr;
		u64 m_PeerId;
	};
	class rlGamerId
	{
	public:
		u64 m_Id;
	};
	#define RL_MAX_BUF_FOR_UTF_CHARS(i) ((i*4)+1) // 4-byte UTF8
	enum
	{
		RL_MAX_LOCAL_GAMERS = 1,
		RL_MAX_NAME_BUF_SIZE = 17,   // social club names can be up to 16 characters + 1 for null terminator
		RL_PRESENCE_MAX_BUF_SIZE = 256,	// For Steam: max value length passed to SetRichPresence() is k_cchMaxRichPresenceValueLength (256 chars)
		RL_MAX_DISPLAY_NAME_BUF_SIZE = RL_MAX_NAME_BUF_SIZE,
		RL_MAX_PARTY_SIZE = 8,
		RL_MAX_NAME_LENGTH = RL_MAX_NAME_BUF_SIZE - 1,
		RL_MAX_INVITE_SUBJECT_CHARS = 16,
		RL_MAX_INVITE_SALUTATION_CHARS = 128,
		RL_MAX_MESSAGE_SUBJECT_CHARS = 18,  // SCE_NP_BASIC_SUBJECT_CHARACTER_MAX
		RL_MAX_MESSAGE_CONTENT_CHARS = 256, // SCE_NP_BASIC_BODY_CHARACTER_MAX (512)
		RL_MAX_MESSAGE_RECIPIENTS = 10,
		RL_MAX_USERID_BUF_LENGTH = 128,
		RL_MAX_URL_BUF_LENGTH = 2048, // ps4 SCE_WEB_BROWSER_DIALOG_URL_SIZE, pc INTERNET_MAX_PATH_LENGTH
		RL_MAX_SERVICE_ABBREV_CHARS = 3,
		//This must be large enough to hold largest result of rlGamerHandle::ToString()
		RL_MAX_GAMER_HANDLE_CHARS =
		RL_MAX_SERVICE_ABBREV_CHARS     //Service id
		+ 1                             //Space
		+ 4                             //NP environment (ticket issuer ID)
		+ 2                             //Space plus " (for NP)
		+ RL_MAX_NAME_BUF_SIZE          //Gamer name
		+ 2                             //" plus space (for NP)
		+ 1,                            //Null terminator
		RL_MAX_GAMERS_PER_SESSION = 32,
		RL_MAX_LEADERBOARD_COLUMNS = 64,
		// Maximum number of chars in a group handle (see rlStats.h, group leaderboards).
		// FIXME(KB) This should be reduced to 32 or 64 when the backend can handle it.
		RL_GROUP_HANDLE_MAX_CHARS = 128,  //Includes the terminating NULL
		//Max length of a filter name, including null terminator.
		RL_MAX_MATCHING_FILTER_NAME_LENGTH = 24,
		//Max length of attribute name, including null terminator.
		RL_MAX_MATCHING_ATTR_NAME_LENGTH = 24,
		//Max number of attributes that can be advertised in matchmaking.
		RL_MAX_MATCHING_ATTRS = 8,
		//Max number of conditions in a matchmaking filter.
		RL_MAX_MATCHING_CONDITIONS = 32,
		//Max size of app-specific data that can be returned with QoS responses
		RL_MAX_QOS_USER_DATA_SIZE = 512,
		//Max size of app-specific data that can be returned with config responses
		RL_MAX_SESSION_USER_DATA_SIZE = 256,
		//Max size of app-specific data-mine that can be returned with config responses
		RL_MAX_SESSION_DATA_MINE_SIZE = 128,
		//Several places in code we do a search for a set of sessions or gather session
		//details from those sessions. This is a common limit that can be 
		//searched/queried/addressed at once.
		RL_FIND_SESSIONS_MAX_SESSIONS_TO_FIND = 20,
		// The RGSC SDK leverages RAGE functionality that often requires a gamer index.
		// In all cases, this gamer index is defined as ZERO.
		RL_RGSC_GAMER_INDEX = 0,
		// Session "Rich Presence" - Sessions on PS4 can contain names, statuses and images.
		RL_MAX_SESSION_NAME_LENGTH = 64,
		RL_MAX_SESSION_NAME_BUF_SIZE = RL_MAX_BUF_FOR_UTF_CHARS(RL_MAX_SESSION_NAME_LENGTH), // 64 UTF-8 characters + null terminator
		RL_MAX_SESSION_STATUS_LENGTH = 128,
		RL_MAX_SESSION_STATUS_BUF_SIZE = RL_MAX_BUF_FOR_UTF_CHARS(RL_MAX_SESSION_STATUS_LENGTH), // 64 UTF-8 characters + null terminator
		RL_MAX_SESSION_IMAGE_BUF_SIZE = (160 * 1024), // 160 KiB
	};
	class rlMatchingFilter
	{
	public:
		u32* GetValue(u32 conditionIndex)
		{
			return (m_ConditionMask & (1 << conditionIndex)) ? &m_ConditionVals[conditionIndex] : NULL;
		}
		u32 GetConditionCount()
		{
			return m_ConditionCount;
		}
		u32 GetGameModeAttribute()
		{
			return ((m_SessionPurpose & 0xFFFF) << 16) | (m_GameMode & 0xFFFF);
		}
		const char* GetSessionAttrFieldNameForCondition(u32 conditionIndex)
		{
			const u32 idx = m_SessionAttrIndicesForConditions[conditionIndex];
			if (!idx)
			{
				return "";
			}
			return m_SessionAttrNames[idx];
		}
		//Id of the filter.  Typically used by the platform back end when
		//performing matchmaking queries.
		u32 m_FilterId;
		char m_FilterName[RL_MAX_MATCHING_FILTER_NAME_LENGTH];
		u32 m_ConditionCount;
		u16 m_GameMode;
		u16 m_SessionPurpose;
		//Attributes advertised by the session host.
		u32 m_SessionAttrIds[RL_MAX_MATCHING_ATTRS];
		char m_SessionAttrNames[RL_MAX_MATCHING_ATTRS][RL_MAX_MATCHING_ATTR_NAME_LENGTH];
		u32 m_SessionAttrCount;
		//For each condition an index into m_SessionAttrIds.  This provides
		//the id of the value on the left side of the condition.  The left side
		//of the condition represents values advertised by the session host.
		u32 m_SessionAttrIndicesForConditions[RL_MAX_MATCHING_CONDITIONS];
		//For each condition the operator used to compare the left side of the
		//condition to the right side.
		u32 m_Operators[RL_MAX_MATCHING_CONDITIONS];
		//For each condition the id of the value on the right side of the
		//condition.  The right side value of the condition is provided when
		//searching for sessions.
		u32 m_ConditionParamIds[RL_MAX_MATCHING_CONDITIONS];
		//For each condition the value on the right side of the condition.
		u32 m_ConditionVals[RL_MAX_MATCHING_CONDITIONS];
		//For each condition a one bit indicates that a right side value
		//has been provided.
		u32 m_ConditionMask;
	};
	class rlRosTitleSecrets
	{
	public:
		enum
		{
			SECRET_VERSION = 11,
			VERSION_OFFSET = 0,
			VERSION_SIZE = 1,
			MASTER_KEY_OFFSET = VERSION_OFFSET + VERSION_SIZE,
			MASTER_SECRET_SIZE = 32,
			KEY_OFFSET = MASTER_KEY_OFFSET + MASTER_SECRET_SIZE,
			KEY_SIZE = 16,
			SALT_OFFSET = KEY_OFFSET + KEY_SIZE,
			SALT_SIZE = 16,
			TOTAL_SIZE = SALT_OFFSET + SALT_SIZE,
			TOTAL_BASE64_SIZE = (((TOTAL_SIZE + 2) / 3) * 4) + 1 //datBase64::GetMaxEncodedSize(TOTAL_SIZE)
		};
		u8 m_Secret[TOTAL_SIZE];
	};
	enum rlRosEnvironment
	{
		// DO NOT CHANGE THE ORDER OF THESE ENVIRONMENTS - BACKWARDS COMPATIBILITY
		RLROS_ENV_UNKNOWN = -1,
		RLROS_ENV_RAGE,
		RLROS_ENV_DEV,
		RLROS_ENV_CERT,
		RLROS_ENV_PROD,
		// ADD NEW ENVIRONMENTS TO THE END OF THIS LIST - BACKWARDS COMPATIBILITY
		RLROS_ENV_CERT_2,
		RLROS_ENV_CI_RAGE
	};
	enum
	{
		RLROS_MAX_PLATFORM_NAME_SIZE = 16,  //Length including terminating null   
		RLROS_MAX_TICKET_SIZE = 512, //String length of base64 ticket data, including terminating null.
		RLROS_MAX_SESSION_TICKET_SIZE = 128, //String length of base64 session ticket data, including terminating null.
		RLROS_MAX_TITLE_NAME_SIZE = 16,  //Length including terminating null   
		RLROS_MAX_TITLE_DIRECTORY_SIZE = 64,  //Length including terminating null   
		RLROS_MAX_USER_NAME_SIZE = 64,  //Max length of a ROS PA UserName, including terminating null.
		RLROS_MAX_USERID_SIZE = 64,  //Max length of a ROS PA UserId, including terminating null.
		RLROS_MAX_RSA_ENCRYPTED_SIZE = 128, //Max size (in bytes) of an encrypted RSA message (size of modulus)
		RLROS_MAX_SERVICE_ACCESS_TOKEN_SIZE = 2048,  //Max size of an Scs Service Access Token, not including terminating null.
		RLROS_MAX_SERVICE_SERVER_URL_SIZE = 128
	};
	//PURPOSE
	//  ROS security settings.
	//  Controls how responses should be encrypted/signed by the server based on
	//  client expectations
	//NOTE: This enum must match the RosSecurityFlags enum on server
	enum rlRosSecurityFlags
	{
		//The original encryption/signature settings. If using RLROS_ENCRYPTION_TITLE:
		//1. Response headers are not signed
		//2. A plaintext sha1 hash of the encrypted block is appended to each encrypted block
		RLROS_SECURITY_NONE = 0,
		//If present, the client will include a random challenge/nonce with the request, which 
		//the server will include with the hmac for the response headers/body. This is intended
		//to be used with the HMAC_HEADERS and HMAC_BODY options and prevents replaying
		//server responses.
		RLROS_SECURITY_CHALLENGE = (1 << 0),
		//Server returns an hmac of certain key headers so that the client can detect any
		//tampering.
		RLROS_SECURITY_HMAC_HEADERS = (1 << 1),
		//Encrypted blocks are followed with an hmac instead of a hash
		RLROS_SECURITY_HMAC_BODY = (1 << 2),
		//If present, the client will request the server to respond with a content signature,
		//if it so chooses. If the server doesn't, then the request will not fail. If it does,
		//then the client will verify that the response body matches the server signature,
		//and fail the request if it doesn't. This is primarily useful for things like a redirect
		//to the CDN, and when used in conjunction with RLROS_SECURITY_HMAC_HEADERS
		//allows the server to determine whether or not to tell the client to verify the signature
		//against the file downloaded from the CDN
		RLROS_SECURITY_REQUEST_CONTENT_SIG = (1 << 3),
		//If present, the client will demand the server to respond with a content signature.
		//If it doesn't, the request should fail, otherwise the client will verify the server's
		//signature.
		RLROS_SECURITY_REQUIRE_CONTENT_SIG = (1 << 4),
		//If present, the client will expect the headers hmac to also include the http status code
		//so that it can't be tampered with
		RLROS_SECURITY_HMAC_STATUS_CODE = (1 << 5),
		//Similar to RLROS_SECURITY_HMAC_HEADERS, this protects the client request header from
		//being tampered with. If present, the client will include an hmac in its request header
		//that is an hmac of other important request headers. The server will verify that the
		//hmac matches expected based on the headers that it received from the client.
		//Likewise, the client will verify the server has done this by including the request
		//header hmac in the response header hmac.
		RLROS_SECURITY_HMAC_REQUEST_HEADERS = (1 << 6),
		//If present, the header hmac should be structured. The header hmac consists of several
		//pieces of the header glued together, allowing someone to move characters between pieces
		//without changing the hmac but potentially altering the meaning of the header. This
		//causes the header hmac to use a pre-defined structure to prevent thi
		RLROS_SECURITY_STRUCTURED_HEADER_HMAC = (1 << 7),
		RLROS_SECURITY_DEFAULT = RLROS_SECURITY_CHALLENGE
		| RLROS_SECURITY_HMAC_HEADERS
		| RLROS_SECURITY_HMAC_BODY
		| RLROS_SECURITY_REQUEST_CONTENT_SIG
		| RLROS_SECURITY_HMAC_STATUS_CODE
		| RLROS_SECURITY_HMAC_REQUEST_HEADERS
		| RLROS_SECURITY_STRUCTURED_HEADER_HMAC
	};
	class rlRosTitleId
	{
	public:
		rlRosTitleId(const char* titleName,
			const char* titleDirectoryName,
			const int scVersion,
			const int titleVersion,
			const char* titleSecretsBase64,
			const rlRosEnvironment env,
			const u8* publicRsaKey,
			const u32 publicRsaKeyLen)
		{}
		char m_TitleName[RLROS_MAX_TITLE_NAME_SIZE];
		int m_TitleVersion;
		int m_ScVersion;
		rlRosSecurityFlags m_SecurityFlags;
		rlRosTitleSecrets m_TitleSecrets;
		const u8* m_PublicRsaKey;
		u32 m_PublicRsaKeyLen;
		char m_TitleSecretsBase64[rlRosTitleSecrets::TOTAL_BASE64_SIZE];
		char m_TitleDirectoryName[RLROS_MAX_TITLE_DIRECTORY_SIZE];
		rlRosEnvironment m_Env;
		char m_DomainName[128];
		char m_CloudDomainName[128];
		char m_HostNamePrefix[64];
		rlRosEnvironment m_ForcedEnv;
	};
	class rlTitleId
	{
	public:
		rlTitleId(const rlRosTitleId& rosTitleId) : m_RosTitleId(rosTitleId)
		{}
		rlRosTitleId m_RosTitleId;
	};
	class rlPc
	{
	public:
		char m_Delegator[32];
		int m_SocialClubVersionNum[4];
		char m_SocialClubVersion[32];
		bool m_bHasScuiRequestedQuit;
		rgsc::IRgscV10* m_Rgsc;
		rgsc::IProfileManagerV2* m_ProfileManager;
		rgsc::IGamepadManagerV3* m_GamepadManager;
		rgsc::IAchievementManagerV3* m_AchievementManager;
		rgsc::IPlayerManagerV2* m_PlayerManager;
		rgsc::IPresenceManagerV4* m_PresenceManager;
		rgsc::ICommerceManagerV4* m_CommerceManager;
		rgsc::IActivationV2* m_ActivationSystem;
		rgsc::IPatchingV1* m_PatchingSystem;
		rgsc::IFileSystemV3* m_FileSystem;
		rgsc::IRgscUiV7* m_RgscUi;
		rgsc::ITaskManagerV1* m_TaskManager;
		rgsc::ITelemetryV3* m_Telemetry;
		rgsc::INetworkV1* m_NetworkInterface;
		rgsc::ICloudSaveManagerV3* m_CloudSaveManager;
		rgsc::IGamerPicManagerV1* m_GamerPicManager;
		HMODULE m_hRgscDll;
		// IPC communication with the game's launcher
		class rlPCPipe* m_PCPipe;
		// Virtual Gamepads
		rgsc::RgscGamepad* m_ScuiPads[ioPad::MAX_PADS];
		u32 m_NumScuiPads;
		char m_RelayDelegate[64];
		char m_EpicDelegate[64];
		int m_InitializationErrorCode;
		bool m_WaitingForKeyboardResult;
		u32 m_KeyboardCompleteTime;
		char m_ScuiVirtualKeyboard[2056];
		char m_AdditionalSessionAttr[rgsc::RGSC_ADDITIONAL_SESSION_ATTR_BUF_SIZE];
		char m_AdditionalJoinAttr[rgsc::RGSC_ADDITIONAL_SESSION_ATTR_BUF_SIZE];
		char m_MetaDataPath[rgsc::RGSC_MAX_PATH];
	};
	enum rlProfileStatsPriorityLevel
	{
		RL_PROFILESTATS_MIN_PRIORITY = 0,
		RL_PROFILESTATS_MAX_PRIORITY = 15
	};
	enum rlProfileStatsType
	{
		RL_PROFILESTATS_TYPE_INVALID = -1,
		RL_PROFILESTATS_TYPE_FLOAT = 3,
		RL_PROFILESTATS_TYPE_INT32 = 4,
		RL_PROFILESTATS_TYPE_INT64 = 0
	};
	class rlProfileStatsValue
	{
	public:
		rlProfileStatsType GetType() const
		{
			return m_Type;
		}
		bool IsValid() const
		{
			return m_Type != RL_PROFILESTATS_TYPE_INVALID;
		}
		void Clear()
		{
			m_Type = RL_PROFILESTATS_TYPE_INVALID;
		}
	private:
		union ValueUnion
		{
			float m_Float;
			s32 m_Int32;
			s64 m_Int64;
		};
		ValueUnion m_Union;
		rlProfileStatsType m_Type;
	};
	typedef u8 rlProfileStatsPriority;
	struct rlProfileDirtyStat
	{
		rlProfileDirtyStat()
			: m_StatId(0)
			, m_StatValue()
			, m_StatPriority(RL_PROFILESTATS_MIN_PRIORITY)
		{}
		rlProfileDirtyStat(const int statId, const rlProfileStatsValue statValue, const rlProfileStatsPriority statPriority)
		{
			Reset(statId, statValue, statPriority);
		}
		int GetStatId() const
		{
			return m_StatId;
		}
		rlProfileStatsPriority GetPriority() const
		{
			return m_StatPriority;
		}
		rlProfileStatsValue GetValue() const
		{
			return m_StatValue;
		}
		void Clear()
		{
			m_StatId = 0;
			m_StatValue.Clear();
			m_StatPriority = RL_PROFILESTATS_MIN_PRIORITY;
		}
		void Reset(const int statId, const rlProfileStatsValue statValue, const rlProfileStatsPriority statPriority)
		{
			m_StatId = statId;
			m_StatValue = statValue;
			m_StatPriority = statPriority;
		}
		rlProfileDirtyStat& operator=(const rlProfileDirtyStat& rhs)
		{
			Reset(rhs.m_StatId, rhs.m_StatValue, rhs.m_StatPriority);
			return *this;
		}
	private:
		int m_StatId;
		rlProfileStatsValue m_StatValue;
		rlProfileStatsPriority m_StatPriority;
	};
	class rlProfileStatsDirtyIterator
	{
	public:
		rlProfileStatsDirtyIterator()
			: m_FlushedCount(0)
		{}
		virtual ~rlProfileStatsDirtyIterator()
		{}
		virtual bool GetCurrentStat(rlProfileDirtyStat& stat) const
		{
			return false;
		}
		virtual void Next()
		{}
		virtual bool AtEnd() const
		{
			return false;
		}
		u32 GetFlushedCount() const
		{
			return m_FlushedCount;
		}
	private:
		friend class rlProfileStatsWriter;
		u32 m_FlushedCount;
	};
	class rlProfileStatsClient
	{
	public:
		friend class rlProfileStats;
		friend class rlProfileStatsFlushTask;
		int m_LocalGamerIndex;
		u32 m_MaxSubmissionSize;
		bool m_SubmitCompressed;
		u32 m_MsUntilNextFlush;
		bool m_FlushEnabled;
		// The default delay between flushes, used until the backend tells us
		// what the delay should be
		const static u32 ms_DefaultFlushDelaySecs = 60 * 5;
	};
	struct rlRosResult
	{
		rlRosResult()
			: m_Succeeded(true), m_Code(0), m_CodeEx(0), m_Ctx(0), m_Location(0), m_Msg(0)
		{}
		bool m_Succeeded;
		const char* m_Code;
		const char* m_CodeEx;
		const char* m_Ctx;
		const char* m_Location;
		const char* m_Msg;
	};
	class Sha1
	{
	public:
		static constexpr u8 SHA1_DIGEST_LENGTH = 20; // 160-bit digest
		static constexpr u8 SHA1_BLOCK_LENGTH = 64;	// 512-bit block
		struct SHA1_CTX
		{
			u32 state[5];
			u64 count;
			u8 buffer[SHA1_BLOCK_LENGTH];
		};
		SHA1_CTX m_Context;
		u8 m_Kpad[Sha1::SHA1_BLOCK_LENGTH];
		u8 m_Key[Sha1::SHA1_BLOCK_LENGTH];
		u32 m_KeyLen;
	};
	class Rc4Context
	{
	public:
		u8 m_X, m_Y;
		u8 m_State[256];
		bool m_IsValid;
	};
	enum rlRosHttpEncryption
	{
		RLROS_ENCRYPTION_NONE,
		RLROS_ENCRYPTION_TITLE
	};
	class rlRosHttpUserAgentHeader
	{
	public:
		enum
		{
			MAX_USER_AGENT_LEN = 160
		};
	private:
		enum
		{
			RANDOM_PREFIX_LEN = 4
		};
		rlRosHttpEncryption m_Encryption;
		char m_TitleName[RLROS_MAX_TITLE_NAME_SIZE];
		char m_PlatformName[RLROS_MAX_PLATFORM_NAME_SIZE];
		s64 m_SessionId;
		PlayerAccountId m_AccountId;
		int m_Version;
		bool m_IsValid : 1;
	};
	template <int MAX_KEY_SIZE>
	struct EncryptionKey
	{
		u8 m_Key[MAX_KEY_SIZE];
		u32 m_KeyLength;
	};
	enum
	{
		RLSC_MAX_AVATAR_URL_CHARS = 160,
		RLSC_MAX_COUNTRY_CODE_CHARS = 3,
		RLSC_MAX_DATE_CHARS = 32,
		RLSC_MAX_EMAIL_CHARS = 101,
		RLSC_MAX_LANGUAGE_ABBREV_CHARS = 10,
		RLSC_MAX_NICKNAME_CHARS = 51,
		RLSC_MAX_PASSWORD_CHARS = 513,
		RLSC_MAX_PHONE_CHARS = 16,
		RLSC_MAX_SCAUTHTOKEN_CHARS = 256,
		RLSC_MAX_ZIP_CODE_CHARS = 10,
		RLSC_MAX_VALID_PASSWORD_SYMBOL_CHARS = 65,
		RLSC_MAX_AUDIENCE_CHARS = 16,
		RLSC_MAX_USERDATA_CHARS = 256,
		RLSC_MAX_MACHINETOKEN_CHARS = 129,
		RLSC_MAX_LINKTOKEN_CHARS = 512
	};
	struct rlScAccountInfo
	{
		int m_Age;
		char m_AvatarUrl[RLSC_MAX_AVATAR_URL_CHARS];
		char m_CountryCode[RLSC_MAX_COUNTRY_CODE_CHARS];
		char m_Dob[RLSC_MAX_DATE_CHARS];
		char m_Email[RLSC_MAX_EMAIL_CHARS];
		bool m_IsApproxDob;
		char m_LanguageCode[RLSC_MAX_LANGUAGE_ABBREV_CHARS];
		char m_Nickname[RLSC_MAX_NICKNAME_CHARS];
		char m_Phone[RLSC_MAX_PHONE_CHARS];
		RockstarId m_RockstarId;
		char m_ZipCode[RLSC_MAX_ZIP_CODE_CHARS];
	};
	enum rlRosPrivilegeId
	{
		RLROS_PRIVILEGEID_NONE = 0, // Unused
		RLROS_PRIVILEGEID_CREATE_TICKET = 1, // Can create ROS tickets
		RLROS_PRIVILEGEID_PROFILE_STATS_WRITE = 2, // Can write profile stats
		RLROS_PRIVILEGEID_MULTIPLAYER = 3, // Can participate in MP sessions (currently not used)
		RLROS_PRIVILEGEID_LEADERBOARD_WRITE = 4, // Can write to leaderboards (currently not used)
		RLROS_PRIVILEGEID_CLOUD_STORAGE_READ = 5, // Can write to cloud storage
		RLROS_PRIVILEGEID_CLOUD_STORAGE_WRITE = 6, // Can read from cloud storage
		RLROS_PRIVILEGEID_BANNED = 7, // Denies all other privileges except CREATE_TICKET
		RLROS_PRIVILEGEID_CLAN = 8, // Can use clan functions
		RLROS_PRIVILEGEID_PRIVATE_MESSAGING = 9, // Can use private messaging system
		RLROS_PRIVILEGEID_SOCIAL_CLUB = 10, // Can login to SC site, and TBD other functionality
		RLROS_PRIVILEGEID_PRESENCE_WRITE = 11, // Can update presence
		RLROS_PRIVILEGEID_DEVELOPER = 12, // If true, user is considered a developer
		RLROS_PRIVILEGEID_HTTP_REQUEST_LOGGING = 13, // If true, user's HTTP request log to SCS gameservices and cloud is logged
		RLROS_PRIVILEGEID_UGCWRITE = 14, // If true, allows writing UGC
		RLROS_PRIVILEGEID_PURCHASEVC = 15, // If true, allows purchasing virtual currency
		RLROS_PRIVILEGEID_TRANSFERVC = 16, // If true, allows transferring virtual currency
		RLROS_PRIVILEGEID_CANBET = 17, // If true, player can place bets
		RLROS_PRIVILEGEID_CREATORS = 18, // If true, player can access the creators
		RLROS_PRIVILEGEID_CLOUD_TUNABLES = 19, // If true, player can download the cloud tunables
		RLROS_PRIVILEGEID_CHEATER_POOL = 20, // if true, player is in the cheater pool
		RLROS_PRIVILEGEID_COMMENTWRITE = 21, // If true, user can create comments
		RLROS_PRIVILEGEID_CANUSELOTTERY = 22, // If true, player can buy lottery tickets
		RLROS_PRIVILEGEID_ALLOW_MEMBER_REDIRECT = 23, // If true, allow member space re-directs
		RLROS_PRIVILEGEID_PLAYED_LAST_GEN = 24, // If true, grant played last gen privilege
		RLROS_PRIVILEGEID_UNLOCK_SPECIAL_EDITION = 25, // If true, grant special edition content
		RLROS_PRIVILEGEID_CONTENT_CREATOR = 26, // If true, allows user to create/update/delete Ugc content bypassing owner and profanity checks 
		RLROS_PRIVILEGEID_CONDUCTOR = 27, // If true, allows user to access game servers (through conductor)
		RLROS_NUM_PRIVILEGEID
	};
	struct rlRosCredentials
	{
		static constexpr u32 SESSION_KEY_SIZE = 32;
		static constexpr u32 CLOUD_KEY_SIZE = 32;
		typedef EncryptionKey<SESSION_KEY_SIZE> SessionKey;
		typedef EncryptionKey<CLOUD_KEY_SIZE> CloudKey;
		struct PrivilegeInfo
		{
			bool m_IsValid : 1;
			bool m_IsGranted : 1;
			bool m_HasEndDate : 1;
			u64 m_EndPosixTime;
		};
		char m_Ticket[RLROS_MAX_TICKET_SIZE];
		char m_SessionTicket[RLROS_MAX_SESSION_TICKET_SIZE];
		u32 m_ValidMs;
		int m_Region;
		PlayerAccountId m_PlayerAccountId;
		s64 m_SessionId;
		rlScAccountInfo m_RockstarAcct;
		atFixedBitSet<RLROS_NUM_PRIVILEGEID> m_Privileges;
	};
	struct rlRosContentSignature
	{
		u8 m_ContentSignature[RLROS_MAX_RSA_ENCRYPTED_SIZE];
		u32 m_Length;
	};
	class rlRosHttpFilter : public netHttpFilter
	{
		struct VerifySignatureDelegate
		{
			char m_Delegate[32]; // Fuck impl for atDelegate. It's so retarded, I refuse to do it. SMD
		};
	public:
		virtual ~rlRosHttpFilter()
		{}
		virtual bool CanFilterRequest() const
		{
			return false;
		}
		virtual bool CanFilterResponse() const
		{
			return false;
		}
		virtual bool CanIntercept() const
		{
			return false;
		}
		virtual u32 DelayMs() const
		{
			return 0;
		}
		virtual bool FilterRequest(const u8* data, const u32 dataLen, const bool finalCall, datGrowBuffer& output)
		{
			return false;
		}
		virtual bool FilterResponse(const u8* data, const u32 dataLen, const bool allDataReceived, fiHandle& outputHandle, const fiDevice* outputDevice, u32* numProcessed, bool* hasDataPending)
		{
			return false;
		}
		virtual const char* GetUserAgentString() const
		{
			return NULL;
		}
		virtual bool ProcessRequestHeader(class netHttpRequest* request)
		{
			return false;
		}
		virtual bool ProcessResponseHeader(const int statusCode, const char* header)
		{}
		virtual void Receive(void* buf, const u32 bufSize, u32* bytesReceived)
		{}
		virtual bool AllowSucceeded(class netHttpRequest* request)
		{
			return false;
		}
		//These constants must EXACTLY match the values used on the backend.
		enum
		{
			KEY_LEN = 16,
			SALT_LEN = 16
		};
		struct RequestData
		{
			rlRosHttpEncryption m_Encryption;
			rlRosSecurityFlags m_OriginalSecurityFlags;
			rlRosSecurityFlags m_SecurityFlags;
			Rc4Context m_Rc4;
			Sha1 m_Sha1;
			bool m_HaveWrittenPreamble;
			char m_UrlToGoTo[512];
			u8 m_Challenge[8];
			u8 m_HeaderHmacDigest[Sha1::SHA1_DIGEST_LENGTH];
		};
		RequestData m_Request;
		struct ResponseData
		{
			enum
			{
				KEY_OFFSET = 0,
				BLOCK_SIZE_OFFSET = KEY_OFFSET + KEY_LEN,
				BLOCK_SIZE_LEN = 4,
				PREAMBLE_LEN = BLOCK_SIZE_OFFSET + BLOCK_SIZE_LEN
			};
			rlRosHttpEncryption m_Encryption;
			rlRosHttpEncryption m_DefaultEncryption;
			Rc4Context m_Rc4;
			u8 m_Key[KEY_LEN];
			bool m_HaveReadPreamble : 1;
			bool m_HaveContentSignature : 1;
			bool m_HaveContentHash : 1;
			u8 m_ExpectedContentHash[Sha1::SHA1_DIGEST_LENGTH];
			rlRosContentSignature m_ExpectedContentSignature;
			Sha1 m_ResponseSha1;
		};
		ResponseData m_Response;
		char m_UserAgentString[rlRosHttpUserAgentHeader::MAX_USER_AGENT_LEN];
		bool m_HaveSessionKey;
		rlRosCredentials::SessionKey m_SessionKey;
		char m_SessionTicket[RLROS_MAX_SESSION_TICKET_SIZE];
		VerifySignatureDelegate m_VerifySignatureDelegate;
		bool m_bDisableHttpDump;
	};
	#define RL_TASK_DECL(name)\
		virtual const char* GetTaskName() const {return #name;}
	#define RL_TASK_USE_CHANNEL(tag)\
		virtual const diagChannel* GetDiagChannel() const {return nullptr;}
	class rlTaskBase
	{
		friend class rlTaskManager;
	public:
		enum FinishType
		{
			FINISH_FAILED,
			FINISH_SUCCEEDED,
			FINISH_CANCELED
		};
		enum ShutdownBehaviour
		{
			CANCEL_ON_SHUTDOWN,
			EXECUTE_ON_SHUTDOWN,
		};
		virtual ~rlTaskBase()
		{}
		virtual const char* GetTaskName() const
		{
			return NULL;
		}
		virtual void Start()
		{}
		virtual void Update(const u32 timeStepMs)
		{}
		virtual bool IsCancelable() const
		{
			return false;
		}
		virtual const diagChannel* GetDiagChannel() const
		{
			return nullptr;
		}
		virtual ShutdownBehaviour GetShutdownBehaviour()
		{
			return CANCEL_ON_SHUTDOWN;
		}
	protected:
		virtual void Finish(const FinishType finishType)
		{}
		virtual void Finish(const FinishType finishType, const int resultCode)
		{}
		virtual void DoCancel()
		{}
	public:
		enum State
		{
			STATE_INVALID = -1,
			STATE_CONFIGURED,
			STATE_STARTED,
			STATE_FINISHED
		};
		enum Flags
		{
			FLAG_CANCELING = 0x01,
			FLAG_CANCELED = 0x02,
		};
		u32 m_Id;
		netStatus* m_Status;
		State m_State;
		u32 m_Flags;
		int m_TimeoutTimerMs;
		//Task manager that's processing this instance.
		class rlTaskManager* m_Owner;
		//Task manager that created this instance.
		class rlTaskManager* m_Creator;
		inmap_node<unsigned, rlTaskBase> m_MapNodeByTaskId;
		inmap_node<const netStatus*, rlTaskBase> m_MapNodeByStatus;
		inmap_node<u64, rlTaskBase> m_MapNodeByQueueId;
		rlTaskBase* m_Next;
		//Prevent copying
		rlTaskBase(const rlTaskBase&);
		rlTaskBase& operator=(const rlTaskBase&);
	};
	class rlTaskManager
	{
		friend class rlTaskBase;
	public:
		sysMemAllocator* m_Allocator;
		typedef inmap<unsigned, rlTaskBase, &rlTaskBase::m_MapNodeByTaskId> TaskMapById;
		typedef inmap<const netStatus*, rlTaskBase, &rlTaskBase::m_MapNodeByStatus> TaskMapByStatus;
		typedef inmultimap<u64, rlTaskBase, &rlTaskBase::m_MapNodeByQueueId> TaskQueue;
		TaskMapById m_TaskMapById;
		TaskMapByStatus m_TaskMapByStatus;
		TaskQueue m_TaskQ;
		u32 m_LastUpdateTime;
		bool m_Initialized : 1;
	};
	template <typename CTX>
	class rlTask : public rlTaskBase
	{
		friend class rlTaskBase;
	public:
		rlTask()
			: m_Ctx(NULL)
		{}
	protected:
		virtual void Finish(const FinishType finishType)
		{
			this->Finish(finishType, 0);
		}
		virtual void Finish(const FinishType finishType, const int resultCode)
		{
			this->rlTaskBase::Finish(finishType, resultCode);
			m_Ctx = NULL;
		}
		//Context on which the task operates.
		CTX* m_Ctx;
	};
	class rlHttpTask : public rlTaskBase
	{
	public:
		static constexpr int MAX_MULTIPART_BOUNDARY_LENGTH = 32;
		static constexpr int MAX_MULTIPART_BOUNDARY_BUF_SIZE = MAX_MULTIPART_BOUNDARY_LENGTH + 1;
		RL_TASK_DECL(rlHttpTask);
		RL_TASK_USE_CHANNEL(rline_http);
		virtual ~rlHttpTask() = default;
		virtual bool IsCancelable() const
		{
			return true;
		}
		virtual void DoCancel()
		{}
		virtual void Start()
		{}
		virtual void Update(const u32 timeStep)
		{}
		virtual void Finish(const FinishType finishType, const int resultCode = 0)
		{}
	protected:
		virtual bool UseHttps() const
		{
			return false;
		}
		virtual SSL_CTX* GetSslCtx() const
		{
			return nullptr;
		}
		virtual const char* GetUrlHostName(char* /*hostnameBuf*/, const u32 /*sizeofBuf*/) const
		{
			return NULL;
		}
		virtual bool GetServicePath(char* /*svcPath*/, const u32 /*maxLen*/) const
		{
			return false;
		}
		virtual bool ProcessResponse(const char* response, int& resultCode)
		{
			return false;
		}
	};
	class rlRosHttpTask : public rlHttpTask
	{
		friend class rlRosSaxReader;
	public:
		RL_TASK_USE_CHANNEL(rline_ros);
		RL_TASK_DECL(rlRosHttpTask);
		virtual ~rlRosHttpTask() = default;
		enum rlRosHttpServices
		{
			GAME_SERVICES,
			LAUNCHER_SERVICES
		};
		virtual bool UseHttps() const
		{
			return false;
		}
		virtual SSL_CTX* GetSslCtx() const
		{
			return nullptr;
		}
		virtual rlRosSecurityFlags GetSecurityFlags() const
		{
			return rlRosSecurityFlags::RLROS_SECURITY_NONE;
		}
		virtual const char* GetUrlHostName(char* hostnameBuf, const u32 sizeofBuf) const
		{
			return NULL;
		}
		virtual bool ProcessResponse(const char* response, int& resultCode)
		{
			return false;
		}
		virtual bool GetServicePath(char* svcPath, const u32 maxLen) const
		{
			return false;
		}
		virtual const char* GetServiceMethod() const
		{
			return NULL;
		}
		virtual bool ProcessSuccess(const rlRosResult& result, const parTreeNode* node, int& resultCode)
		{
			return false;
		}
		virtual void ProcessError(const rlRosResult& result, const parTreeNode* node, int& resultCode)
		{}
		virtual bool SaxStartElement(const char* name)
		{
			return false;
		}
		virtual bool SaxAttribute(const char* name, const char* val)
		{
			return false;
		}
		virtual bool SaxCharacters(const char* ch, const int start, const int length)
		{
			return false;
		}
		virtual bool SaxEndElement(const char* name)
		{
			return false;
		}
		virtual bool UseFilter() const
		{
			return false;
		}
		virtual const char* GetServiceSet() const
		{
			return NULL;
		}
		rlRosHttpFilter m_Filter;
	private:
		int m_LocalGamerIndex;
		rlRosHttpServices m_Service;
		parTree** m_ParserTree;
	};
	class rlProfileStatsWriteStatsTask : public rlRosHttpTask
	{
	public:
		RL_TASK_DECL(rlProfileStatsWriteStatsTask);
		RL_TASK_USE_CHANNEL(rline_profilestats);
		virtual ~rlProfileStatsWriteStatsTask()
		{}
	protected:
		virtual const char* GetServiceMethod() const
		{
			return "ProfileStats.asmx/WriteStats";
		}
		virtual bool ProcessSuccess(const rlRosResult& result, const parTreeNode* node, int& resultCode)
		{
			return false;
		}
	private:
		friend class rlProfileStatsFlushTask;
		int* m_NumWritten;
		int* m_SecsUntilNextWrite;
		int* m_MaxSubmissionBinarySize;
	};
	class rlProfileStatsFlushTask : public rlTask<rlProfileStatsClient>
	{
	public:
		RL_TASK_DECL(rlProfileStatsFlushTask);
		RL_TASK_USE_CHANNEL(rline_profilestats);
		virtual ~rlProfileStatsFlushTask()
		{}
		virtual void Start()
		{}
		virtual void Update(const u32 timestep)
		{}
	protected:
		virtual void Finish(const FinishType finishType, const int resultCode = 0)
		{}
		virtual bool IsCancelable() const
		{
			return true;
		}
		virtual void DoCancel()
		{}
	private:
		rlProfileStatsWriteStatsTask m_WriteStatsTask;
		int m_NumWritten;
		int m_SecsUntilNextWrite;
		int m_MaxSubmissionBinarySize;
		netStatus m_MyStatus;
	};
}
//scrXXXXXX
namespace rage
{
	enum scrProgramId
	{
		srcpidNONE
	};
	union scrValue
	{
		enum ValueType
		{
			UNKNOWN, BOOL, INT, FLOAT, VECTOR, TEXT_LABEL, STRING, OBJECT
		};
		enum VarArgType
		{
			VA_INT, VA_FLOAT, VA_STRINGPTR, VA_VECTOR
		};
		int Int;
		unsigned Uns;
		float Float;
		const char* String;
		scrValue* Reference;
		size_t Any;
		bool operator==(const scrValue& that)
		{
			return Int == that.Int;
		}
	};
	typedef scrValue* scrReference;
	typedef size_t scrAny;
	enum scrThreadId : int
	{
		THREAD_INVALID = 0,
	};
	class scrVector
	{
	public:
		scrVector()
		{}
		scrVector(float x, float y, float z) : x(x), y(y), z(z)
		{}
		scrVector(const Vector3& v) : x(v.x), y(v.y), z(v.z)
		{}

		void set(float sx, float sy, float sz)
		{
			x = sx;
			y = sy;
			z = sz;
		}
		scrVector& operator=(const Vector3& v)
		{
			x = v.x; y = v.y; z = v.z;
			return *this;
		}
		operator Vector3() const
		{
			return Vector3(x, y, z);
		}

		float x, xPad, y, yPad, z, zPad;
	};
#define MAX_CALLSTACK 16
	class scrThread
	{
	public:
		enum
		{
			c_DefaultStackSize = 512
		}; // static constexpr causes missing symbol under gcc 4.1.1 debug builds.
// Presume it's something to do with its use as a default parameter.
		enum
		{
			c_NativeInsnLength = 4
		}; // Length of OP_NATIVE insn in case caller needs PC of *next* insn.
		enum State
		{
			RUNNING, BLOCKED, ABORTED, HALTED
		};
		enum Priority
		{
			THREAD_PRIO_HIGHEST, THREAD_PRIO_NORMAL, THREAD_PRIO_LOWEST, THREAD_PRIO_MANUAL_UPDATE = 100
		};
		struct Serialized
		{
			scrThreadId m_ThreadId; // +0 Unique thread ID (always increasing)
			scrProgramId m_Prog; // +4 Stored by hashcode rather than pointer so load/save can work
			State m_State; // +8 Current thread state
			int m_PC; // +12 Current program counter (index into program's opcodes array)
			int m_FP; // +16 Current frame pointer (anchor for local variables)
			int m_SP; // +20 Stack pointer
			float m_TimerA, m_TimerB; // Local per-thread timer values
			float m_Wait; // Accumulated wait timer
			int m_MinPC, m_MaxPC; // Step region for debugger
			atRangeArray<scrValue, 4> m_TLS; // Thread-local storage for blocking functions
			int m_StackSize;
			int m_CatchPC, m_CatchFP, m_CatchSP;
			Priority m_Priority; // 0=highest (runs first), 2=lowest (runs last)
			s8 m_CallDepth;
			atRangeArray<int, MAX_CALLSTACK> m_CallStack;
		};
		struct Global
		{
			int BasicType; // enumerant in scrValue
			int Offset; // Base address of this variable
			u32 Hash; // Hashed name of this variable (as per scrComputeHash)
		};
		struct Info
		{
			Info(scrValue* resultPtr, int parameterCount, const scrValue* params) : ResultPtr(resultPtr), ParamCount(parameterCount), Params(params), BufferCount(0)
			{}
			// Return result, if applicable
			scrValue* ResultPtr;
			// Parameter count
			int ParamCount;
			// Pointer to parameter values
			const scrValue* Params;
			// Temp storage for managing copyin/copyout of Vector3 parameters
			int BufferCount;
			enum
			{
				MAX_VECTOR3 = 4
			};
			scrValue* Orig[MAX_VECTOR3];
			scrVector Buffer[MAX_VECTOR3];
			scrVector& GetVector3(int& N)
			{
				scrValue* v = Params[N++].Reference;
				Orig[BufferCount] = v;
				Buffer[BufferCount].set(v[0].Float, v[1].Float, v[2].Float);
			}
			void CopyReferencedParametersOut()
			{
				int bc = BufferCount;
				while (bc--)
				{
					Orig[bc][0].Float = Buffer[bc].x;
					Orig[bc][1].Float = Buffer[bc].y;
					Orig[bc][2].Float = Buffer[bc].z;
				}
			}
		};
		virtual ~scrThread() = default;
		virtual void Reset(scrProgramId program, const void* argStruct, int argStructSize)
		{}
		virtual State Run(int insnCount)
		{
			return m_Serialized.m_State;
		}
		virtual State Update(int insnCount)
		{
			return m_Serialized.m_State;
		}
		virtual void Kill()
		{}
		struct ThreadStack
		{
			scrThread* m_Owner;
			int m_StackSize;
			scrValue* m_Stack;
		};
		Serialized m_Serialized;
		scrValue* m_Stack;
		int	m_iInstructionCount;
		int m_argStructSize;
		int m_argStructOffset;
		const char* m_AbortReason;
		u32 m_ScriptNameHash;
		char m_ScriptName[64];
	};
	typedef void(*scrCmd)(scrThread::Info*);
	const int MAX_LEGIT_OPS = (11 * 1024 * 1024); //	Used as the size of the atFixedArray called Program in rage\script\tools\scriptcompiler\node.h

	// These configure the size of each opcode page
	static constexpr u32 scrPageShift = 14;
	static constexpr u32 scrPageSize = (1 << scrPageShift);
	static constexpr u32 scrPageMask = (scrPageSize - 1);

	// These configure the size of each string page.
	static constexpr u32 scrStringShift = 14;
	static constexpr u32 scrStringSize = (1 << scrStringShift);
	static constexpr u32 scrStringMask = (scrStringSize - 1);

	// These configure the size of each globals page, in multiples of sizeof(scrValue).  This is used to prevent globals from requiring an absurdly large memory block just long enough to (re)load.
	static constexpr u32 scrGlobalsPageShift = 14;
	static constexpr u32 scrGlobalsPageSize = (1 << scrGlobalsPageShift);
	static constexpr u32 scrGlobalsPageMask = (scrGlobalsPageSize - 1);

	class scrProgram : public pgBase
	{
		friend class scrThread;
	public:
		static const int RORC_VERSION = 12;

		enum
		{
			MAX_GLOBAL_BLOCKS_SHIFT = (24 - 6),
			MAX_GLOBAL_BLOCKS = (64),
			GLOBAL_SIZE_MASK = ((1 << (MAX_GLOBAL_BLOCKS_SHIFT)) - 1)
		};

		u8** Table; // +8 (zero if compiled)
		u32 GlobalsHash; // +12
		u32 OpcodeSize;	// +16
		u32 ArgStructSize; // +20
		u32 StaticSize;	// +24
		u32 GlobalSizeAndBlock;// +28 - upper bits are the globals block index
		u32 NativeSize; // +32
		scrValue* Statics; // +36
		scrValue** GlobalsTable; // +40 (zero if compiled, globals cannot be declared in a compiled script)
		u64* Natives; // +44 (if compiled, really a script function pointer)
		u32 ProcCount; // +48 (zero if compiled)
		const char** ProcNames; // +52 (zero if compiled)
		u32 HashCode; // +56
		u32 RefCount; // +60
		ConstString ScriptName; // +64
		const char** StringHeaps; // +68 (zero if compiled)
		u32 StringHeapSize; // +72 (zero if compiled)
		atMap<s32, bool>* m_programBreakpoints; // +76 (zero if compiled)

		u8* GetCode(u32 index)
		{
			if (index < OpcodeSize)
			{
				return &Table[index >> scrPageShift][index & scrPageMask];
			}
			return nullptr;
		}
		const scrValue* GetStatics() const
		{
			return Statics;
		}
		int GetStaticSize() const
		{
			return StaticSize;
		}
		int GetStringHeapSize() const
		{
			return StringHeapSize;
		}
		u32 GetHashCode() const
		{
			return HashCode;
		}
		u32 GetNativeSize() const
		{
			return NativeSize;
		}
		const u64* GetNatives() const
		{
			return Natives;
		}
		u32 GetStringHeapCount() const
		{
			return (StringHeapSize + scrStringMask) >> scrStringShift;
		}
		u32 GetStringHeapChunkSize(u32 i)
		{
			return i == GetStringHeapCount() - 1 ? StringHeapSize - (i << scrStringShift) : scrStringSize;
		}
		u32 GetGlobalsPageCount() const
		{
			return ((GlobalSizeAndBlock & GLOBAL_SIZE_MASK) + scrGlobalsPageMask) >> scrGlobalsPageShift;
		}
		u32 GetGlobalsPageChunkSize(u32 i)
		{
			return i == GetGlobalsPageCount() - 1 ? (GlobalSizeAndBlock & GLOBAL_SIZE_MASK) - (i << scrGlobalsPageShift) : scrGlobalsPageSize;
		}
	};
	template <typename T>
	class scrCommandHash
	{
	private:
		static constexpr int ToplevelSize = 256; // Must be power of two
		static constexpr int PerBucket = 7;
		struct Bucket
		{
			sysObfuscated<Bucket*, false> obf_Next;
			T Data[PerBucket];
			sysObfuscated<u32, false> obf_Count;
			sysObfuscated<u64, false> obf_Hashes[PerBucket];
			u64 plainText_Hashes[PerBucket];
		};
	public:
		void RegistrationComplete(bool val)
		{
			m_bRegistrationComplete = val;
		}
		void Init()
		{
			m_Occupancy = 0;
			m_bRegistrationComplete = false;
			for (int i{}; i < ToplevelSize; i++)
				m_Buckets[i] = NULL;
		}
		void Kill()
		{
			for (int i = 0; i < ToplevelSize; i++)
			{
				Bucket* b = m_Buckets[i];
				while (b)
				{
					char* old = (char*)b;
					b = b->obf_Next.Get();
					delete[] old;
				}
				m_Buckets[i] = NULL;
			}
			m_Occupancy = 0;
		}
		T Lookup(u64 hashcode)
		{
			Bucket* b = m_Buckets[hashcode & (ToplevelSize - 1)];
			while (b)
			{
				for (u32 i{}; i != b->obf_Count.Get(); i++)
					if (b->obf_Hashes[i].Get() == hashcode)
						return b->Data[i];
				b = b->obf_Next.Get();
			}
			return T{};
		}
		Bucket* m_Buckets[ToplevelSize];
		int m_Occupancy;
		bool m_bRegistrationComplete;
	};
	class tlsContext
	{
	public:
		char gap0[180];
		std::uint8_t m_unk_byte; // 0xB4
		char gapB5[3];
		sysMemAllocator* m_allocator; // 0xB8
		sysMemAllocator* m_allocator2; // 0xC0 - Same as 0xB8
		sysMemAllocator* m_allocator3; // 0xC8 - Same as 0xB8
		uint32_t m_console_smth; // 0xD0
		char gapD4[188];
		uint64_t m_unk; // 0x190
		char gap198[1728];
		scrThread* m_script_thread; // 0x858
		bool m_is_script_thread_active; // 0x860

		static tlsContext* get()
		{
			constexpr std::uint32_t TlsIndex = 0x0;
			return *reinterpret_cast<tlsContext**>(__readgsqword(0x58) + TlsIndex);
		}
	};
	// script object ID defines
	class scriptIdBase
	{
	public:
		scriptIdBase() {}
		virtual ~scriptIdBase() {}
		//PURPOSE
		// Initialises the script id from a script thread
		virtual void Init(const scrThread* scriptThread)
		{}
		//PURPOSE
		// Returns true if the script id has been initialised
		virtual bool IsValid() const
		{
			return false;
		}
		//PURPOSE
		// Generates a hash to uniquely identify this script name
		virtual const atHashValue GetScriptNameHash() const
		{
			return {};
		}
		//PURPOSE
		// Generates a hash to uniquely identify this script id
		virtual atHashValue GetScriptIdHash() const
		{
			return {};
		}
		//PURPOSE
		// Returns the string used to represent the script id in the network logs
		virtual const char* GetLogName() const
		{
			return "";
		}
		//PURPOSE
		// Reads the script id data from a bit buffer
		virtual void Read(datBitBuffer* bb)
		{}
		//PURPOSE
		// Writes the script id data to a bit buffer
		virtual void Write(datBitBuffer* bb) const
		{}
		//PURPOSE
		// Returns the current size of the script id data
		virtual unsigned GetSizeOfMsgData() const
		{
			return 0;
		}
		//PURPOSE
		// Returns the maximum size of the script id data
		virtual unsigned GetMaximumSizeOfMsgData() const 
		{
			return 0;
		}
		//PURPOSE
		// Logs the script id data in a network log
		virtual void Log(netLoggingInterface* log)
		{}
	protected:
		virtual void From(const scriptIdBase* other)
		{}
		virtual bool Equals(const scriptIdBase* other) const
		{
			return false;
		}
		virtual void SetNameFromHash()
		{}
	};
	#define SIZEOF_HANDLER_OBJECT_ID (16)
	#define INVALID_SCRIPT_OBJECT_ID (0)
	#define SCRIPT_OBJECT_HOST_SLOT_INDEX (255)			// the slot index assigned to host created objects
	#define SCRIPT_OBJECT_UNNETWORKED_SLOT_INDEX (254)	// the slot index assigned to non-networked script objects
	#define MAKE_SCRIPT_OBJECT_ID(slot, index) (slot<<SIZEOF_HANDLER_OBJECT_ID | index); // builds a composite script object ID based on the specified slot and index
	#define GET_SLOT_FROM_SCRIPT_OBJECT_ID(scriptObjectID) (scriptObjectID>>SIZEOF_HANDLER_OBJECT_ID)
	#define GET_INDEX_FROM_SCRIPT_OBJECT_ID(scriptObjectID)(scriptObjectID&((1<<SIZEOF_HANDLER_OBJECT_ID)-1))
	#define IS_HOST_OBJECT_ID(scriptObjectID) (GET_SLOT_FROM_SCRIPT_OBJECT_ID(scriptObjectID)==SCRIPT_OBJECT_HOST_SLOT_INDEX)
	#define IS_UNNETWORKED_OBJECT_ID(scriptObjectID) (GET_SLOT_FROM_SCRIPT_OBJECT_ID(scriptObjectID)==SCRIPT_OBJECT_UNNETWORKED_SLOT_INDEX)
	#define SIZEOF_SCRIPT_OBJECT_ID (32)
	typedef u32 ScriptObjectId;
	typedef s16 ScriptParticipantId;
	typedef s16 ScriptSlotNumber;
	typedef u16 HostToken;

	class scriptObjInfoBase
	{
	public:
		virtual ~scriptObjInfoBase() {}
		virtual scriptIdBase& GetScriptId() = 0;
		virtual const scriptIdBase& GetScriptId() const	= 0;
		ScriptObjectId GetObjectId() const { return m_ScriptObjectId; }
		HostToken GetHostToken() const { return m_HostToken; }
		bool IsValid() const { return m_ScriptObjectId != INVALID_SCRIPT_OBJECT_ID; }
		void Invalidate() { m_ScriptObjectId = INVALID_SCRIPT_OBJECT_ID; }
	protected:
		static const unsigned SIZEOF_HOST_TOKEN = 3;
		ScriptObjectId m_ScriptObjectId; // the ID assigned to this object from a script handler
		HostToken m_HostToken; // the token of the host that created this object
	};
	class scriptHandler;
	class netObject;
	class fwEntity;
	class scriptHandlerObject
	{
	public:

		virtual ~scriptHandlerObject()
		{}
		virtual unsigned GetType() const = 0;
		virtual void CreateScriptInfo(const scriptIdBase& scrId, const ScriptObjectId& objectId, const HostToken hostToken) = 0;
		virtual void SetScriptInfo(const scriptObjInfoBase& scrInfo) = 0;
		virtual scriptObjInfoBase* GetScriptInfo() = 0;
		virtual const scriptObjInfoBase* GetScriptInfo() const = 0;
		virtual void SetScriptHandler(scriptHandler* handler) = 0;
		virtual scriptHandler* GetScriptHandler() const = 0;
		virtual void ClearScriptHandler() = 0;
		virtual void OnRegistration(bool newObject, bool hostObject) = 0;
		virtual void OnUnregistration() = 0;
		virtual bool IsCleanupRequired() const = 0;
		virtual void Cleanup() = 0;
		virtual netObject* GetNetObject() const = 0;
		virtual fwEntity* GetEntity() const = 0;
		virtual bool HostObjectCanBeCreatedByClient() const
		{
			// if true clients are allowed to create host objects locally - this is used for certain types of objects that are all declared locally on every machine when the script starts up
			return false;
		}
		bool IsHostObject() const
		{
			return IS_HOST_OBJECT_ID(GetScriptInfo()->GetObjectId());
		}
		bool IsUnnetworkedObject() const
		{
			return IS_UNNETWORKED_OBJECT_ID(GetScriptInfo()->GetObjectId());
		}
	};
	typedef u32 ScriptResourceId;
	typedef u16 ScriptResourceType;
	typedef s32 ScriptResourceRef;
	class scriptResource
	{
		friend scriptHandler;
	public:
		static const int INVALID_RESOURCE_ID = 0; // invalid ids must be 0 (which is also NULL in the scripts)
		virtual ~scriptResource()
		{}
		ScriptResourceType GetType() const
		{
			return m_Type;
		}
		ScriptResourceId GetId() const
		{
			return m_Id;
		}
		void SetId(ScriptResourceId id)
		{
			m_Id = id;
		}
		const ScriptResourceRef GetReference() const
		{
			return m_Reference;
		}
		void SetReference(ScriptResourceRef ref)
		{
			m_Reference = ref;
		}
		virtual const char* GetResourceName() const = 0;
		virtual ScriptResourceRef GetInvalidReference() const = 0;
		virtual scriptResource* Clone() const = 0;
		virtual bool Create() = 0;
		virtual void Destroy() = 0;
		virtual void Detach()
		{
		}
		virtual bool DetachOnCleanup() const
		{
			return false;
		}
		virtual bool LeaveForOtherScripts() const
		{
			return false;
		}
	protected:
		ScriptResourceType m_Type; // the type of the resource
		ScriptResourceId m_Id; // the id of the resource assigned by the script handler it is associated with. Unique to this resource instance.
		ScriptResourceRef m_Reference; // the reference used to identify the resource
		inlist_node<scriptResource> m_ListLink;
	};
	// There are 2 indices for each player, the active and physical index. The active index is the index into the player manager's player pile and is 
	// not consistent across the network - i.e. our player will not have the same active player index on other machines. The physical player index is the 
	// index into the player manager's physical array and is consistent across the network. This index has a smaller range and is used in various places 
	// in the network code (eg to set a flag in a network object's cloned state).
	typedef u8 ActivePlayerIndex;
	typedef u8 PhysicalPlayerIndex;
	typedef u16 ObjectId;
	typedef u16 NetObjFlags;
	typedef u32 PlayerFlags;
	typedef u16 TeamFlags;
	typedef u16 NetworkArrayHandlerType;
	typedef u16 NetworkEventType;
	typedef u16 NetworkObjectType;
	typedef u32 DataNodeFlags;
	static const ActivePlayerIndex MAX_NUM_ACTIVE_PLAYERS = 32;
	static const PhysicalPlayerIndex MAX_NUM_PHYSICAL_PLAYERS = 32;
	static const PhysicalPlayerIndex MAX_NUM_SCRIPT_PARTICIPANTS = 32;
	#define MAX_NUM_NETOBJPLAYERS (MAX_NUM_PHYSICAL_PLAYERS)
	#define MAX_NUM_NETOBJPEDS (110)
	#define MAX_NUM_LOCAL_NETOBJPEDS (100)
	#define MAX_NUM_NETOBJVEHICLES (160)
	#define MAX_NUM_LOCAL_NETOBJVEHICLES (120)
	#define MAX_NUM_NETOBJOBJECTS (80)
	#define MAX_NUM_NETOBJPICKUPS (80)
	#define MAX_NUM_NETOBJPICKUPPLACEMENTS (50)
	#define MAX_NUM_NETOBJGLASSPANE (50)
	#define MAX_NUM_NETOBJECTS (MAX_NUM_NETOBJPLAYERS + MAX_NUM_NETOBJPEDS + MAX_NUM_NETOBJVEHICLES + MAX_NUM_NETOBJOBJECTS + MAX_NUM_NETOBJPICKUPS + MAX_NUM_NETOBJPICKUPPLACEMENTS + MAX_NUM_NETOBJGLASSPANE + MAX_NUM_NETOBJDOORS)
	#define MAX_NUM_ACTIVE_NETWORK_SCRIPTS (50)
	#define MAX_NUM_TEAMS (16)
	#define MAX_NUM_PARTY_MEMBERS (RL_MAX_PARTY_SIZE)
	#define MAX_NUM_NETOBJDOORS (20)
	enum
	{
		//Maximum number of chars in the full name of an auto id.
		//Includes null terminator.
		AUTOID_MAX_LENOF_FULLNAME  = 255,
	};
	class AutoIdDescriptor
	{
		friend class AutoIdRegistry;
	public:
		virtual ~AutoIdDescriptor() {}
		unsigned m_Level;
		unsigned m_Id;
		int m_Ordinal;
		char m_Name[10];
		const AutoIdDescriptor* m_Family;
		AutoIdDescriptor* m_Next;
		bool m_HasMandatedId;
		char m_Priority;
	};
	typedef AutoIdDescriptor AutoIdFamily;
	class AutoIdRegistry
	{
		friend class AutoIdMgr;
	public:
		AutoIdDescriptor* m_Head;
		AutoIdDescriptor** m_OrderedIds;
		int m_Count;
		int m_MandatedIdCount;
		unsigned m_FirstConsecutiveId;
		unsigned m_LastConsecutiveId;
		AutoIdRegistry* m_Next;
	};
	template <typename T>
	class AutoIdRegistrar
	{
	public:

	};
	template <typename T>
	class AutoIdDescriptor_T : public AutoIdDescriptor
	{
	public:
		virtual ~AutoIdDescriptor_T() = default;
	};
	class AutoIdMgr
	{
	public:
		struct IdRegCollection
		{
			IdRegCollection()
				: m_Head(0),
				m_Count(0)
			{}
			AutoIdRegistry* m_Head;
			int m_Count;
		};
	};
	#define AUTOID_DECL_COMMON( name )\
		static const name::AutoIdDesc__ AutoId__;\
		static const name::AutoIdDesc__* GetAudoIdDesc() { return &AutoId__; }\
		static unsigned GetAutoId() { return AutoId__.GetId(); }
	#define AUTOID_DECL_ID( name, family, id )\
		typedef family::AutoIdRoot AutoIdRoot;\
		enum { AutoIdLevel  = family::AutoIdLevel + 1 };\
		struct AutoIdDesc__ : public ::rage::AutoIdDescriptor_T< AutoIdRoot >\
		{\
			AutoIdDesc__() : ::rage::AutoIdDescriptor_T< AutoIdRoot >( AutoIdLevel, id, family::GetAudoIdDesc() ) {}\
		};\
		AUTOID_DECL_COMMON(name);
	#define NET_MESSAGE_EXPORT_COMMON(func)\
		template<typename T>\
		bool Export(T& bb) const\
		{\
			return rage::netMessage::WriteHeader(this->GetMsgId(), bb)\
					&& this->func(bb, *this);\
		}\
		bool Export(void* buf, const unsigned sizeofBuf, unsigned* size = 0) const\
		{\
			rage::datExportBuffer bb;\
			bb.SetReadWriteBytes(buf, sizeofBuf);\
			const bool success = this->Export(bb);\
			if(size){*size = success ? bb.GetNumBytesWritten() : 0;}\
			return success;\
		}
	#define NET_MESSAGE_IMPORT_COMMON(func)\
		template<typename T>\
		bool Import(T& bb)\
		{\
			unsigned msgId = 0;\
			return rage::netMessage::ReadHeader(&msgId, bb)\
					&& this->GetMsgId() == msgId\
					&& this->func(bb, *this);\
		}\
		bool Import(const void* buf, const unsigned sizeofBuf, unsigned* size = 0)\
		{\
			rage::datImportBuffer bb;\
			bb.SetReadOnlyBytes(buf, sizeofBuf);\
			const bool success = this->Import(bb);\
			if(size){*size = success ? bb.GetNumBytesRead() : 0;}\
			return success\
					/*Make sure we read all the bits in the msg.*/\
					&& (bb.GetNumBytesRead() == (int) sizeofBuf);\
		}
	#define NET_MESSAGE_IMPORT_STUB_COMMON(func)\
		template<typename T>\
		bool ImportStub(T& bb)\
		{\
			unsigned msgId = 0;\
			return rage::netMessage::ReadHeader(&msgId, bb)\
					&& this->func(bb, *this);\
		}\
		bool ImportStub(const void* buf, const unsigned sizeofBuf, unsigned* size = 0)\
		{\
			rage::datImportBuffer bb;\
			bb.SetReadOnlyBytes(buf, sizeofBuf);\
			const bool success = this->ImportStub(bb);\
			if(size){*size = success ? bb.GetNumBytesRead() : 0;}\
				return success\
						/*Make sure we read all the bits in the msg.*/\
						&& (bb.GetNumBytesRead() < (int) sizeofBuf);\
		}
	#define NET_MESSAGE_DECL_COMMON(name)\
		static unsigned MSG_ID() { return name::GetAutoId(); }\
		unsigned GetMsgId() const { return name::GetAutoId(); }\
		NET_MESSAGE_EXPORT_COMMON(SerMsgPayload)\
		NET_MESSAGE_IMPORT_COMMON(SerMsgPayload)
	#define NET_MESSAGE_DECL(name, id)\
		AUTOID_DECL_ID(name, rage::netMessage, id)\
		NET_MESSAGE_DECL_COMMON(name)
	class msgScriptJoin;
	class msgScriptJoinAck;
	class msgScriptJoinHostAck;
	class msgScriptHandshake;
	class msgScriptHandshakeAck;
	class msgScriptLeave;
	class msgScriptLeaveAck;
	class msgScriptBotJoin;
	class msgScriptBotJoinAck;
	class msgScriptBotHandshake;
	class msgScriptBotHandshakeAck;
	class msgScriptBotLeave;
	class msgScriptMigrateHost;
	class msgScriptMigrateHostFailAck;
	class msgScriptHostRequest;
	class msgScriptNewHost;
	class msgScriptVerifyHost;
	class msgScriptVerifyHostAck;
	class scriptHandlerNetComponent
	{
	public:
		static const unsigned MAX_NUM_NET_COMPONENTS = MAX_NUM_ACTIVE_NETWORK_SCRIPTS;
		static const int INVALID_PARTICIPANT_ID = -1;
		static const int INVALID_SLOT = -1;
		static const int INVALID_HOST_TOKEN = 0;
		static const int TIME_HOSTING_BEFORE_FIRST_HOST_MIGRATION = 60000; // the length of time a new host will host the script session before attempting to migrate the host
		static const int TIME_BETWEEN_HOST_MIGRATION_ATTEMPTS = 10000; // the length of time between host migration attempts
		static const int TIME_BETWEEN_HOST_VERIFY_ATTEMPTS = 1000; // the length of time between local host verify attempts
		static const int TIME_BETWEEN_HOST_REQUESTS = 250; // the length of time between the sending of host request messages
		enum eRejectionReason 
		{
			REJECTED_INVALID = -1,
			REJECTED_CANNOT_PROCESS,
			REJECTED_NO_ROOM_IN_SCRIPT_SESSION,
			REJECTED_NO_JOIN_IN_PROGRESS,
			REJECTED_NO_ROOM_IN_TEAM,
			NUM_REJECTION_REASONS
		};
		virtual ~scriptHandlerNetComponent() = default;
		virtual bool Update() = 0;
		virtual void PlayerHasJoined(const netPlayer* player) = 0;
		virtual void PlayerHasLeft(const netPlayer* player) = 0;
		virtual void MigrateScriptHost(const netPlayer* player) = 0;
		virtual void HandleJoinMsg(const msgScriptJoin* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleJoinAckMsg(const msgScriptJoinAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleJoinHostAckMsg(const msgScriptJoinHostAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleHandshakeMsg(const msgScriptHandshake* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleHandshakeAckMsg(const msgScriptHandshakeAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleLeaveMsg(const msgScriptLeave* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleLeaveAckMsg(const msgScriptLeaveAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleBotJoinMsg(const msgScriptBotJoin* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleBotJoinAckMsg(const msgScriptBotJoinAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleBotHandshakeMsg(const msgScriptBotHandshake* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleBotHandshakeAckMsg(const msgScriptBotHandshakeAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleBotLeaveMsg(const msgScriptBotLeave* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleMigrateHostMsg(const msgScriptMigrateHost* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleMigrateHostFailAckMs(const msgScriptMigrateHostFailAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleHostRequestMsg(const msgScriptHostRequest* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleNewHostMsg(const msgScriptNewHost* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleVerifyHostMsg(const msgScriptVerifyHost* msg, const ReceivedMessageData* messageData) = 0;
		virtual void HandleVerifyHostAckMsg(const msgScriptVerifyHostAck* msg, const ReceivedMessageData* messageData) = 0;
		virtual void RegisterHostBroadcastData(unsigned* address, unsigned size, bool HasHighFrequencyUpdates = false) = 0;
		virtual void RegisterPlayerBroadcastData(unsigned* address, unsigned size, bool HasHighFrequencyUpdates = false) = 0;
		virtual bool CanStartJoining() = 0;
		virtual bool CanStartHosting() = 0;
		virtual bool TryToAddPlayerAsParticipant(const netPlayer* player, ScriptParticipantId* participantId, ScriptSlotNumber* slotNumber, eRejectionReason* rejection) = 0;
		virtual bool AddPlayerAsParticipant(const netPlayer* player, ScriptParticipantId participantId, ScriptSlotNumber slotNumber) = 0;
	};
	class scriptHandler
	{
	public:
		class atDScriptObjectNode : public atDNode<scriptHandlerObject*, datBase>
		{
		public:
		};
		typedef inlist<scriptResource, &scriptResource::m_ListLink> ResourceList;
		virtual ~scriptHandler() = 0;
		virtual bool Update() = 0;
		virtual bool NetworkUpdate() = 0;
		virtual void Shutdown() = 0;
		virtual scriptIdBase& GetScriptId() = 0;
		virtual const scriptIdBase&	GetScriptId() const = 0;
		scrThread* GetThread() { return m_Thread; }
		const scrThread* GetThread() const { return m_Thread; }
		const char* GetLogName() const { return GetScriptId().GetLogName(); }
		scriptHandlerNetComponent* GetNetworkComponent() const{ return m_NetComponent; }
		bool IsTerminated() const { return m_Thread == NULL; }
		void SetNextFreeHostObjectId(ScriptObjectId id) { m_NextFreeHostObjectId = id; }
		virtual bool RequiresANetworkComponent() const = 0;
		virtual void CreateNetworkComponent() = 0;
		virtual void DestroyNetworkComponent() = 0;
		virtual bool Terminate() = 0;
		virtual bool RegisterNewScriptObject(scriptHandlerObject &object, bool hostObject, bool networked) = 0;
		virtual void UnregisterScriptObject(scriptHandlerObject &object) = 0;
		virtual scriptResource* RegisterScriptResource(scriptResource& resource, bool *pbResourceHasJustBeenAddedToList = NULL) = 0;
		virtual bool RemoveScriptResource(ScriptResourceId resourceId, bool bDetach = false, bool bAssert = true, ScriptResourceType resourceType = (ScriptResourceType)-1) = 0;
		virtual bool RemoveScriptResource(ScriptResourceType type, const ScriptResourceRef ref, bool bDetach = false, bool bAssert = true) = 0;
	protected:
		virtual ScriptResourceId GetNextFreeResourceId(scriptResource& resource) { return ++m_NextFreeResourceId; }
		virtual void RegisterExistingScriptObjectInternal(scriptHandlerObject &object) = 0;
		virtual void CleanupScriptObject(scriptHandlerObject &object) = 0;
		virtual bool DestroyScriptResource(scriptResource& resource) = 0;
		virtual void RemoveOldScriptObject(scriptHandlerObject &object) = 0;
	public:
		inlist_node<scriptHandler> m_ListLink; // The list link used by the script handler list in the script handler manager.
	protected:
		scrThread* m_Thread; // the thread this handler is associated with
		atDList<scriptHandlerObject*, datBase> m_ObjectList; // a list of objects created by the script associated with this handler
		ResourceList m_ResourceList; // a list of resources that have been created by the script
		scriptHandlerNetComponent* m_NetComponent; // the network component, which exists during a network game 
		ScriptObjectId m_NextFreeHostObjectId; // the next free id to be assigned to a host script object created by the script associated with this handler.
		ScriptObjectId m_NextFreeClientObjectId; // the next free id to be assigned to a client script object created by the script associated with this handler.
		ScriptObjectId m_NextFreeNonNetworkedObjectId; // the next free id to be assigned to a non-networked script object created by the script associated with this handler.
		ScriptResourceId m_NextFreeResourceId; // the next free id to be assigned to a script resource created by the script associated with this handler.
	private:
		scriptHandler(const scriptHandler&) {}
	};

	class scriptId : public scriptIdBase
	{
	public:
		static const unsigned SCRIPT_NAME_LEN = 32;
		static const unsigned SIZEOF_NAME_HASH = 32;

		atHashValue	m_ScriptNameHash;
		char m_ScriptName[SCRIPT_NAME_LEN];
	};
}
struct SContentLockData
{
	static const u8 MAX_CONTENT_LOCK_SUBSYSTEM_CBS = 15; // can store up to MAX_CONTENT_LOCK_SUBSYSTEM_CBS indices, will need to bump m_callBackIndices storage type
	static const s8 INVALID_CONTENT_LOCK_CB_INDEX = -1;
	bool m_locked : 1;
	u16 m_callBackIndices : MAX_CONTENT_LOCK_SUBSYSTEM_CBS;
};
struct SLoadingScreenState
{
	void Reset()
	{
		m_modified = false;
		m_displayed = false;
		m_doneAPostFrame = false;
	}
	bool m_modified : 1; // Did we change anything?
	bool m_displayed : 1; // Did we display them?
	bool m_doneAPostFrame : 1; // Have we done one whole frame after the load?
};
enum eMapChangeStates
{
	MCS_INVALID = -1,
	MCS_NONE,
	MCS_INIT,
	MCS_UPDATE,
	MCS_END,
	MCS_COUNT
};
struct sOverlayInfo
{
	u32 m_content;
	rage::atHashString m_nameId;
	rage::atHashString m_changeSet;
	rage::atHashString m_changeSetGroupToExecuteWith;
	enum State
	{
		INACTIVE,
		WILL_ACTIVATE,
		ACTIVE,
		NUM_OVERLAY_STATES,
	};
	State m_state;
	s8 m_version;
};
struct ContentChangeSetGroup
{
	rage::atHashString m_NameHash;
	rage::atArray<rage::atHashString> m_ContentChangeSets;
};
enum eExtraContentPackType
{
	EXTRACONTENT_TU_PACK,
	EXTRACONTENT_LEVEL_PACK,
	EXTRACONTENT_COMPAT_PACK,
	EXTRACONTENT_PAID_PACK
};
struct SSetupData
{
	rage::atString m_deviceName;
	rage::atString m_datFile;
	rage::atString m_timeStamp;
	rage::atFinalHashString m_nameHash;
	rage::atArray<rage::atHashString> m_contentChangeSets;
	rage::atArray<ContentChangeSetGroup> m_contentChangeSetGroups;
	rage::atFinalHashString m_startupScript;
	s32 m_scriptCallstackSize;
	eExtraContentPackType m_type;
	s32	m_order;
	s32 m_minorOrder;
	rage::atHashString m_dependencyPackHash;
	rage::atString m_requiredVersion;
	bool m_isLevelPack;
	int	m_subPackCount;
};
struct SActiveChangeSet
{
	rage::atHashString m_group;
	rage::atHashString m_changeSetName;
};
class CMountableContent
{
public:
	enum eDeviceType
	{
		DT_INVALID = 0, DT_FOLDER, DT_PACKFILE, DT_XCONTENT, DT_DLC_DISC, DT_NUMTYPES
	};
	enum eContentStatus
	{
		CS_NONE = 0, CS_SETUP_READ, CS_FULLY_MOUNTED, CS_CORRUPTED, CS_INVALID_SETUP, CS_SETUP_MISSING, CS_UPDATE_REQUIRED, CS_COUNT
	};
	enum eMountResult
	{
		MR_MOUNTED = 0, MR_INIT_FAILED, MR_MOUNT_FAILED, MR_CONTENT_CORRUPTED
	};
	rage::fiDevice* m_pDevice;
	rage::fiDeviceCrc* m_pCrcDevice;
	rage::fiDeviceCrc* m_pCrcDeviceTU;
	rage::fiDeviceRelative* m_pDeviceTU;
	rage::atArray<rage::fiPackfile*> m_subPacks;
	SSetupData m_setupData;
	rage::atString m_filename;
	eDeviceType m_deviceType;
	s32 m_packFileECIndex;
	eContentStatus m_status;
	rage::atArray<SActiveChangeSet> m_activeChangeSets;
	bool m_datFileLoaded : 1;
	bool m_usesPackfile : 1;
	bool m_permanent : 1;
	bool m_isShippedContent : 1;
};
class CExtraContentManager : public rage::datBase
{
public:
	enum eSpecialTrigger
	{
		ST_XMAS = 1 << 0
	};
	rage::atArray<sOverlayInfo*> m_overlayInfo;
	rage::atArray<sOverlayInfo*> m_pendingOverlays;
	rage::atArray<CMountableContent> m_content;
	rage::atArray<rage::atString> m_corruptedContent;
	rage::atArray<u32> m_savegameInstalledPackages;
	rage::atBinaryMap<SContentLockData, u32> m_contentLocks;
	rage::atFixedArray<char[32], SContentLockData::MAX_CONTENT_LOCK_SUBSYSTEM_CBS> m_onContentLockStateChangedCBs;
	rage::rlGamerId m_currentGamerId;
	s32 m_localGamerIndex;
	eMapChangeStates m_currMapChangeState;
	SLoadingScreenState m_loadingScreenState;
};
class CNetworkAssetVerifier
{
public:
	bool m_bJapaneseBuild;
	rage::sysObfuscated<u32> m_fileChecksum;
	rage::sysObfuscated<u32> m_numFiles;
	rage::sysObfuscated<u32> m_interiorProxyChecksum;
	rage::sysObfuscated<u32> m_interiorProxyCount;
	rage::sysObfuscated<u32> m_nLocalTunablesCRC;
	rage::sysObfuscated<u32> m_scriptSize;
	rage::sysObfuscated<u32> m_scriptStoreHash;
	rage::sysObfuscated<u32> m_scriptRpfHash;
	rage::sysObfuscated<u32> m_memoryTamperCRC;
	rage::sysObfuscated<u32> m_nBackgroundScriptCRC;
	rage::sysObfuscated<u32> m_EnvironmentCRC;
	rage::sysObfuscated<int> m_CRC;
	rage::sysObfuscated<int> m_StaticCRC;
};
class CGameScriptId : public scriptId
{
public:
	static const unsigned SIZEOF_TIME_STAMP = 32;
	static const unsigned SIZEOF_POSITION_HASH = 32;
	static const unsigned SIZEOF_INSTANCE_ID = 8;
	virtual ~CGameScriptId()
	{}
	unsigned m_TimeStamp; // the time this script started, set by the host
	unsigned m_PositionHash; // a hash generated from the script's position (only used by ambient scripts)
	int m_InstanceId; // an id that is used to distinguish between different instances of the same script	
	atHashValue	m_scriptIdHash; // the hash identifying this id
};
class CGameScriptHandler : public scriptHandler
{
public:
	static const unsigned MAX_NUM_SCRIPT_HANDLERS = 83;
	virtual bool Update() = 0;
	virtual void Shutdown() = 0;
	virtual bool Terminate() = 0;
	virtual bool RequiresANetworkComponent() const { return false; }
	virtual void CreateNetworkComponent() = 0;
	virtual scriptResource* RegisterScriptResource(scriptResource* resource, bool *pbResourceHasJustBeenAddedToList = NULL) = 0;
	virtual bool RemoveScriptResource(ScriptResourceId resourceId, bool bDetach = false, bool bAssert = true, ScriptResourceType resourceType = (ScriptResourceType)-1) = 0;
	virtual bool RemoveScriptResource(ScriptResourceType type, const ScriptResourceRef ref, bool bDetach = false, bool bAssert = true) = 0;
	virtual ScriptResourceId GetNextFreeResourceId(scriptResource* resource) = 0;
	virtual bool DestroyScriptResource(scriptResource* resource) = 0;
	virtual void CleanupScriptObject(scriptHandlerObject* object) = 0;
	CGameScriptId m_ScriptId;
};
class CGameScriptHandlerNetComponent : public scriptHandlerNetComponent
{
public:
	virtual void RemovePlayerAsParticipant(const netPlayer* player)
	{}
	virtual void HandleLeavingPlayer(const netPlayer* player, bool playerLeavingSession)
	{}
	virtual void SetNewScriptHost(const netPlayer* pPlayer, HostToken hostToken, bool bBroadcast = true)
	{}
	virtual bool CanStartJoining()
	{
		return false;
	}
	virtual void RestartJoiningProcess()
	{}
	virtual void StartPlaying()
	{}
	virtual bool DoLeaveCleanup()
	{
		return false;
	}
	virtual bool DoTerminationCleanup()
	{
		return false;
	}
	PlayerFlags	m_JoinEventFlags;
	unsigned m_PendingTimestamp;
	bool m_bScriptReady : 1;
	bool m_bStartedPlaying: 1;
	bool m_bDoneLeaveCleanup : 1;
	bool m_bDoneTerminationCleanup : 1;
	bool m_bBroadcastScriptInfo : 1;
};
class GtaThread : public scrThread
{
public:
	static const int FORCE_CLEANUP_FLAG_PLAYER_KILLED_OR_ARRESTED = 1;
	static const int FORCE_CLEANUP_FLAG_SP_TO_MP = 2;
	static const int FORCE_CLEANUP_PAUSE_MENU_TERMINATED = 4;
	enum MatchingMode
	{
		MATCH_SCRIPT_NAME,
		MATCH_THREAD_ID,
		MATCH_ALL
	};
	CGameScriptHandler* m_Handler;
	CGameScriptHandlerNetComponent* m_NetComponent;
	s32 m_HashOfScriptName;
	int ForceCleanupPC, ForceCleanupFP, ForceCleanupSP;
	u32 m_ForceCleanupFlags;
	u32 m_CauseOfMostRecentForceCleanup;
	int InstanceId;
	int EntityIndexForScriptBrain;
	bool IsThisAMissionScript; // TRUE for a script which contains a mission
	bool bSafeForNetworkGame; // TRUE if the script should persist when a network game is started/ended
	bool bSetPlayerControlOnInMissionCleanup; //	TRUE (default) if the player control should be switched on when a proper mission ends
	bool bClearHelpInMissionCleanup; //	TRUE (default) if the help text should be cleared when a proper mission ends
	bool bIsThisAMiniGameScript; // Set by a script Command - used to decide which script can display help messages when a mini-game is in progress
	bool bThisScriptAllowsNonMiniGameHelpMessages;
	bool bThreadHasBeenPaused; // This script should not be executed as it has been paused (by PAUSE_GAME)
	bool bThisScriptCanBePaused; // If false, PAUSE_GAME does not affect this script
	bool bThisScriptCanRemoveBlipsCreatedByAnyScript;
	bool m_bDisplayProfileData;
	bool m_bIsHighPrio;
	u8 m_ForceCleanupTriggered;
	s8 ScriptBrainType;
	u32 m_bHasUsedPedPathCommand : 1;
	u32 m_bHasOverriddenVehicleEntering : 1;
	u32 m_bIsARandomEventScript : 1;
	u32 m_bIsATriggerScript : 1;
	bool m_bExitFlagSet : 1;
	bool m_bExitFlagResponse : 1;
	bool m_bCleanupReplay : 1;
};
class CStreamedScripts : public rage::fwAssetRscStore<rage::scrProgram, rage::fwAssetNameDef<rage::scrProgram>>
{
public:
	virtual strLocalIndex Register(const char* name) = 0;
	virtual u32 GetStreamerReadFlags() const = 0;
	virtual void RemoveRef(strLocalIndex index, strRefKind kind) = 0;
	virtual int GetNumRefs(strLocalIndex index) const = 0;
	rage::strIndex m_BGScriptPackStreamIndex;
};
struct sBailParameters
{
	eBailReason m_BailReason;
	int m_ErrorCode;
	int m_ContextParam1;
	int m_ContextParam2;
	int m_ContextParam3;
	uint64_t m_EndPosixTime;
	const char* m_ErrorString;
	const char* m_PassThroughString;
	static const unsigned MAX_NUMBERS = 3;
	int m_Numbers[MAX_NUMBERS];
	unsigned m_nNumbers;
	unsigned m_Flags;
};