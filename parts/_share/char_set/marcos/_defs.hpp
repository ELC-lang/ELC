//_defs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#define U_MASK(x) ((uint32_t)1<<(x))

#define GET_CATEGORY(props) ((props)&0x1f)

#define CAT_MASK(props) U_MASK(GET_CATEGORY(props))

#define _UTRIE_INDEX_FROM_SUPP(trieIndex, c) \
	(((int32_t)((trieIndex)[\
		(trieIndex)[(UTRIE_INDEX_1_OFFSET-UTRIE_OMITTED_BMP_INDEX_1_LENGTH)+\
					((c)>>UTRIE_SHIFT_1)]+\
		(((c)>>UTRIE_SHIFT_2)&UTRIE_INDEX_2_MASK)])\
	<<UTRIE_INDEX_SHIFT)+\
	((c)&UTRIE_DATA_MASK))

#define _UTRIE_INDEX_RAW(offset, trieIndex, c) \
	(((int32_t)((trieIndex)[(offset)+((c)>>UTRIE_SHIFT_2)])\
	<<UTRIE_INDEX_SHIFT)+\
	((c)&UTRIE_DATA_MASK))

#define _UTRIE_INDEX_FROM_CP(trie, asciiOffset, c) \
	((uint32_t)(c)<0xd800 ?\
		_UTRIE_INDEX_RAW(0, (trie)->index, c) :\
		(uint32_t)(c)<=0xffff ?\
			_UTRIE_INDEX_RAW(\
				(c)<=0xdbff ? UTRIE_LSCP_INDEX_2_OFFSET-(0xd800>>UTRIE_SHIFT_2) : 0,\
				(trie)->index, c) :\
			(uint32_t)(c)>0x10ffff ?\
				(asciiOffset)+UTRIE_BAD_UTF8_DATA_OFFSET :\
				(c)>=(trie)->highStart ?\
					(trie)->highValueIndex :\
					_UTRIE_INDEX_FROM_SUPP((trie)->index, c))

#define _UTRIE_GET(trie, data, asciiOffset, c) (trie)->data[_UTRIE_INDEX_FROM_CP(trie, asciiOffset, c)]

#define UTRIE_GET16(trie, c) _UTRIE_GET((trie), index, (trie)->indexLength, (c))

#define GET_PROPS(c, result) ((result)=UTRIE_GET16(&propstrie, c))

//file_end

