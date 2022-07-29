#pragma once

#include <vector>

using namespace std;

// class Tを最大MAXSIZE個確保可能なPoolAllocatorを実装してください
template<class T, size_t MAXSIZE> class PoolAllocator {
private:
    // TODO: 実装してください
    union element_type {
        T data;
        element_type* next;
    };

    vector<element_type> buffer;
    element_type* freeList;
public:

	// コンストラクタ
	PoolAllocator() {
		// TODO: 必要に応じて実装してください
        // buffer を MAXSIZE 分確保する
        buffer = vector<element_type>(MAXSIZE);
        // 連続したメモリ領域を確保するため
        // buffer[i].next に i+1 番目の要素のアドレスを代入する
        for (size_t i = 0; i < MAXSIZE - 1; i++) {
            buffer[i].next = reinterpret_cast<element_type*>(&buffer[i + 1]);
        }
        // 最後の要素の next に nullptr を代入する
        buffer[MAXSIZE - 1].next = nullptr;
        // freeList に buffer[0] のアドレスを代入する
        freeList = &buffer[0];
	}

	// デストラクタ
	~PoolAllocator() {
		// TODO: 必要に応じて実装してください
        buffer.clear();
	}

	// 確保できない場合はnullptrを返す事。
	T* Alloc() {
		// TODO: 実装してください
        // 確保できない、つまり、空きのリストが nullptr である場合は nullptr を返す
        if (freeList == nullptr) return nullptr;

        // res に freeList->data のアドレスを代入する
        T* res = &freeList->data;
        // freeList に freeList->next のアドレスを代入する
        // freeList 1つ消費、できなければ nullptr を代入する
        auto fn = freeList->next;
        if (fn == nullptr)
            freeList = nullptr;
        else
            freeList = fn;

        return res;
	}

	// Free(nullptr)で誤動作しないようにする事。
	void Free(T* addr) {
		// TODO: 実装してください
        // nullptr で誤作動しないようにする
        if (addr == nullptr) return;

        // addr を element_type* にキャストする
        auto* p = reinterpret_cast<element_type*>(addr);
        // p->next に freeList のアドレスを代入する
        // freeList 1つ追加
        p->next = freeList;
        // freeList に p のアドレスを代入する
        freeList = p;
	}
};
