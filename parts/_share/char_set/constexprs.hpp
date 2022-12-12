//constexprs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
inline namespace constexprs{
	inline namespace offsets{
		/** Shift size for getting the index-1 table offset. */
		inline constexpr auto UTRIE_SHIFT_1=6+5;

		/** Shift size for getting the index-2 table offset. */
		inline constexpr auto UTRIE_SHIFT_2=5;

		/**
		 * Difference between the two shift sizes,
		 * for getting an index-1 offset from an index-2 offset. 6=11-5
		 */
		inline constexpr auto UTRIE_SHIFT_1_2=UTRIE_SHIFT_1-UTRIE_SHIFT_2;

		/**
		 * Number of index-1 entries for the BMP. 32=0x20
		 * This part of the index-1 table is omitted from the serialized form.
		 */
		inline constexpr auto UTRIE_OMITTED_BMP_INDEX_1_LENGTH=0x10000>>UTRIE_SHIFT_1;

		/** Number of code points per index-1 table entry. 2048=0x800 */
		inline constexpr auto UTRIE_CP_PER_INDEX_1_ENTRY=1<<UTRIE_SHIFT_1;

		/** Number of entries in an index-2 block. 64=0x40 */
		inline constexpr auto UTRIE_INDEX_2_BLOCK_LENGTH=1<<UTRIE_SHIFT_1_2;

		/** Mask for getting the lower bits for the in-index-2-block offset. */
		inline constexpr auto UTRIE_INDEX_2_MASK=UTRIE_INDEX_2_BLOCK_LENGTH-1;

		/** Number of entries in a data block. 32=0x20 */
		inline constexpr auto UTRIE_DATA_BLOCK_LENGTH=1<<UTRIE_SHIFT_2;

		/** Mask for getting the lower bits for the in-data-block offset. */
		inline constexpr auto UTRIE_DATA_MASK=UTRIE_DATA_BLOCK_LENGTH-1;

		/**
		 * Shift size for shifting left the index array values.
		 * Increases possible data size with 16-bit index values at the cost
		 * of compactability.
		 * This requires data blocks to be aligned by UTRIE_DATA_GRANULARITY.
		 */
		inline constexpr auto UTRIE_INDEX_SHIFT=2;

		/** The alignment size of a data block. Also the granularity for compaction. */
		inline constexpr auto UTRIE_DATA_GRANULARITY=1<<UTRIE_INDEX_SHIFT;

		/* Fixed layout of the first part of the index array. ------------------- */

		/**
		 * The BMP part of the index-2 table is fixed and linear and starts at offset 0.
		 * Length=2048=0x800=0x10000>>UTRIE_SHIFT_2.
		 */
		inline constexpr auto UTRIE_INDEX_2_OFFSET=0;

		/**
		 * The part of the index-2 table for U+D800..U+DBFF stores values for
		 * lead surrogate code _units_ not code _points_.
		 * Values for lead surrogate code _points_ are indexed with this portion of the table.
		 * Length=32=0x20=0x400>>UTRIE_SHIFT_2. (There are 1024=0x400 lead surrogates.)
		 */
		inline constexpr auto UTRIE_LSCP_INDEX_2_OFFSET=0x10000>>UTRIE_SHIFT_2;
		inline constexpr auto UTRIE_LSCP_INDEX_2_LENGTH=0x400>>UTRIE_SHIFT_2;

		/** Count the lengths of both BMP pieces. 2080=0x820 */
		inline constexpr auto UTRIE_INDEX_2_BMP_LENGTH=UTRIE_LSCP_INDEX_2_OFFSET+UTRIE_LSCP_INDEX_2_LENGTH;

		/**
		 * The 2-byte UTF-8 version of the index-2 table follows at offset 2080=0x820.
		 * Length 32=0x20 for lead bytes C0..DF, regardless of UTRIE_SHIFT_2.
		 */
		inline constexpr auto UTRIE_UTF8_2B_INDEX_2_OFFSET=UTRIE_INDEX_2_BMP_LENGTH;
		inline constexpr auto UTRIE_UTF8_2B_INDEX_2_LENGTH=0x800>>6;  /* U+0800 is the first code point after 2-byte UTF-8 */

		/**
		 * The index-1 table, only used for supplementary code points, at offset 2112=0x840.
		 * Variable length, for code points up to highStart, where the last single-value range starts.
		 * Maximum length 512=0x200=0x100000>>UTRIE_SHIFT_1.
		 * (For 0x100000 supplementary code points U+10000..U+10ffff.)
		 *
		 * The part of the index-2 table for supplementary code points starts
		 * after this index-1 table.
		 *
		 * Both the index-1 table and the following part of the index-2 table
		 * are omitted completely if there is only BMP data.
		 */
		inline constexpr auto UTRIE_INDEX_1_OFFSET=UTRIE_UTF8_2B_INDEX_2_OFFSET+UTRIE_UTF8_2B_INDEX_2_LENGTH;
		inline constexpr auto UTRIE_MAX_INDEX_1_LENGTH=0x100000>>UTRIE_SHIFT_1;

		/*
		* Fixed layout of the first part of the data array. -----------------------
		* Starts with 4 blocks (128=0x80 entries) for ASCII.
		*/

		/**
		 * The illegal-UTF-8 data block follows the ASCII block, at offset 128=0x80.
		 * Used with linear access for single bytes 0..0xbf for simple error handling.
		 * Length 64=0x40, not UTRIE_DATA_BLOCK_LENGTH.
		 */
		inline constexpr auto UTRIE_BAD_UTF8_DATA_OFFSET=0x80;

		/** The start of non-linear-ASCII data blocks, at offset 192=0xc0. */
		inline constexpr auto UTRIE_DATA_START_OFFSET=0xc0;
	}
	inline namespace categorys{
		/** Non-category for unassigned and non-character code points. @stable ICU 2.0 */
		inline constexpr auto U_UNASSIGNED = 0;
		/** Cn "Other, Not Assigned (no characters in [UnicodeData.txt] have this property)" (same as U_UNASSIGNED!) @stable ICU 2.0 */
		inline constexpr auto U_GENERAL_OTHER_TYPES = 0;
		/** Lu @stable ICU 2.0 */
		inline constexpr auto U_UPPERCASE_LETTER = 1;
		/** Ll @stable ICU 2.0 */
		inline constexpr auto U_LOWERCASE_LETTER = 2;
		/** Lt @stable ICU 2.0 */
		inline constexpr auto U_TITLECASE_LETTER = 3;
		/** Lm @stable ICU 2.0 */
		inline constexpr auto U_MODIFIER_LETTER = 4;
		/** Lo @stable ICU 2.0 */
		inline constexpr auto U_OTHER_LETTER = 5;
		/** Mn @stable ICU 2.0 */
		inline constexpr auto U_NON_SPACING_MARK = 6;
		/** Me @stable ICU 2.0 */
		inline constexpr auto U_ENCLOSING_MARK = 7;
		/** Mc @stable ICU 2.0 */
		inline constexpr auto U_COMBINING_SPACING_MARK = 8;
		/** Nd @stable ICU 2.0 */
		inline constexpr auto U_DECIMAL_DIGIT_NUMBER = 9;
		/** Nl @stable ICU 2.0 */
		inline constexpr auto U_LETTER_NUMBER = 10;
		/** No @stable ICU 2.0 */
		inline constexpr auto U_OTHER_NUMBER = 11;
		/** Zs @stable ICU 2.0 */
		inline constexpr auto U_SPACE_SEPARATOR = 12;
		/** Zl @stable ICU 2.0 */
		inline constexpr auto U_LINE_SEPARATOR = 13;
		/** Zp @stable ICU 2.0 */
		inline constexpr auto U_PARAGRAPH_SEPARATOR = 14;
		/** Cc @stable ICU 2.0 */
		inline constexpr auto U_CONTROL_CHAR = 15;
		/** Cf @stable ICU 2.0 */
		inline constexpr auto U_FORMAT_CHAR = 16;
		/** Co @stable ICU 2.0 */
		inline constexpr auto U_PRIVATE_USE_CHAR = 17;
		/** Cs @stable ICU 2.0 */
		inline constexpr auto U_SURROGATE = 18;
		/** Pd @stable ICU 2.0 */
		inline constexpr auto U_DASH_PUNCTUATION = 19;
		/** Ps @stable ICU 2.0 */
		inline constexpr auto U_START_PUNCTUATION = 20;
		/** Pe @stable ICU 2.0 */
		inline constexpr auto U_END_PUNCTUATION = 21;
		/** Pc @stable ICU 2.0 */
		inline constexpr auto U_CONNECTOR_PUNCTUATION = 22;
		/** Po @stable ICU 2.0 */
		inline constexpr auto U_OTHER_PUNCTUATION = 23;
		/** Sm @stable ICU 2.0 */
		inline constexpr auto U_MATH_SYMBOL = 24;
		/** Sc @stable ICU 2.0 */
		inline constexpr auto U_CURRENCY_SYMBOL = 25;
		/** Sk @stable ICU 2.0 */
		inline constexpr auto U_MODIFIER_SYMBOL = 26;
		/** So @stable ICU 2.0 */
		inline constexpr auto U_OTHER_SYMBOL = 27;
		/** Pi @stable ICU 2.0 */
		inline constexpr auto U_INITIAL_PUNCTUATION = 28;
		/** Pf @stable ICU 2.0 */
		inline constexpr auto U_FINAL_PUNCTUATION = 29;
	}
	inline namespace masks{
		inline constexpr auto U_GC_CN_MASK = U_MASK(U_GENERAL_OTHER_TYPES);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_LU_MASK = U_MASK(U_UPPERCASE_LETTER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_LL_MASK = U_MASK(U_LOWERCASE_LETTER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_LT_MASK = U_MASK(U_TITLECASE_LETTER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_LM_MASK = U_MASK(U_MODIFIER_LETTER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_LO_MASK = U_MASK(U_OTHER_LETTER);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_MN_MASK = U_MASK(U_NON_SPACING_MARK);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_ME_MASK = U_MASK(U_ENCLOSING_MARK);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_MC_MASK = U_MASK(U_COMBINING_SPACING_MARK);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_ND_MASK = U_MASK(U_DECIMAL_DIGIT_NUMBER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_NL_MASK = U_MASK(U_LETTER_NUMBER);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_NO_MASK = U_MASK(U_OTHER_NUMBER);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_ZS_MASK = U_MASK(U_SPACE_SEPARATOR);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_ZL_MASK = U_MASK(U_LINE_SEPARATOR);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_ZP_MASK = U_MASK(U_PARAGRAPH_SEPARATOR);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_CC_MASK = U_MASK(U_CONTROL_CHAR);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_CF_MASK = U_MASK(U_FORMAT_CHAR);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_CO_MASK = U_MASK(U_PRIVATE_USE_CHAR);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_CS_MASK = U_MASK(U_SURROGATE);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PD_MASK = U_MASK(U_DASH_PUNCTUATION);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PS_MASK = U_MASK(U_START_PUNCTUATION);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PE_MASK = U_MASK(U_END_PUNCTUATION);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PC_MASK = U_MASK(U_CONNECTOR_PUNCTUATION);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PO_MASK = U_MASK(U_OTHER_PUNCTUATION);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_SM_MASK = U_MASK(U_MATH_SYMBOL);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_SC_MASK = U_MASK(U_CURRENCY_SYMBOL);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_SK_MASK = U_MASK(U_MODIFIER_SYMBOL);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_SO_MASK = U_MASK(U_OTHER_SYMBOL);

		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PI_MASK = U_MASK(U_INITIAL_PUNCTUATION);
		/** Mask constant for a UCharCategory. @stable ICU 2.1 */
		inline constexpr auto U_GC_PF_MASK = U_MASK(U_FINAL_PUNCTUATION);


		/** Mask constant for multiple UCharCategory bits (L Letters). @stable ICU 2.1 */
		inline constexpr auto U_GC_L_MASK = (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK|U_GC_LM_MASK|U_GC_LO_MASK);

		/** Mask constant for multiple UCharCategory bits (LC Cased Letters). @stable ICU 2.1 */
		inline constexpr auto U_GC_LC_MASK = (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK);

		/** Mask constant for multiple UCharCategory bits (M Marks). @stable ICU 2.1 */
		inline constexpr auto U_GC_M_MASK = (U_GC_MN_MASK|U_GC_ME_MASK|U_GC_MC_MASK);

		/** Mask constant for multiple UCharCategory bits (N Numbers). @stable ICU 2.1 */
		inline constexpr auto U_GC_N_MASK = (U_GC_ND_MASK|U_GC_NL_MASK|U_GC_NO_MASK);

		/** Mask constant for multiple UCharCategory bits (Z Separators). @stable ICU 2.1 */
		inline constexpr auto U_GC_Z_MASK = (U_GC_ZS_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK);

		/** Mask constant for multiple UCharCategory bits (C Others). @stable ICU 2.1 */
		inline constexpr auto U_GC_C_MASK = (U_GC_CN_MASK|U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CO_MASK|U_GC_CS_MASK);

		/** Mask constant for multiple UCharCategory bits (P Punctuation). @stable ICU 2.1 */
		inline constexpr auto U_GC_P_MASK = (U_GC_PD_MASK|U_GC_PS_MASK|U_GC_PE_MASK|U_GC_PC_MASK|U_GC_PO_MASK|U_GC_PI_MASK|U_GC_PF_MASK);

		/** Mask constant for multiple UCharCategory bits (S Symbols). @stable ICU 2.1 */
		inline constexpr auto U_GC_S_MASK = (U_GC_SM_MASK|U_GC_SC_MASK|U_GC_SK_MASK|U_GC_SO_MASK);
	}
	inline namespace chars{
		inline constexpr auto TAB =0x0009;
		inline constexpr auto LF =0x000a;
		inline constexpr auto FF =0x000c;
		inline constexpr auto CR =0x000d;
		inline constexpr auto NBSP =0x00a0;
		inline constexpr auto CGJ =0x034f;
		inline constexpr auto FIGURESP=0x2007;
		inline constexpr auto HAIRSP =0x200a;
		inline constexpr auto ZWNJ =0x200c;
		inline constexpr auto ZWJ =0x200d;
		inline constexpr auto RLM =0x200f;
		inline constexpr auto NNBSP =0x202f;
		inline constexpr auto ZWNBSP =0xfeff;
	}
}

//file_end

