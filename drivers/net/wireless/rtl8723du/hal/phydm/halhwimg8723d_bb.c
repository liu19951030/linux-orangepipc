// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2007 - 2017 Realtek Corporation */

/*Image2HeaderVersion: 3.5.2*/
#include "mp_precomp.h"
#include "phydm_precomp.h"

static bool
check_positive(
	struct PHY_DM_STRUCT *p_dm,
	const u32	condition1,
	const u32	condition2,
	const u32	condition3,
	const u32	condition4
)
{
	u8	_board_type = ((p_dm->board_type & BIT(4)) >> 4) << 0 | /* _GLNA*/
			((p_dm->board_type & BIT(3)) >> 3) << 1 | /* _GPA*/
			((p_dm->board_type & BIT(7)) >> 7) << 2 | /* _ALNA*/
			((p_dm->board_type & BIT(6)) >> 6) << 3 | /* _APA */
			((p_dm->board_type & BIT(2)) >> 2) << 4 | /* _BT*/
			((p_dm->board_type & BIT(1)) >> 1) << 5 | /* _NGFF*/
			((p_dm->board_type & BIT(5)) >> 5) << 6;  /* _TRSWT*/

	u32	cond1 = condition1, cond2 = condition2, cond3 = condition3, cond4 = condition4;

	u8	cut_version_for_para = (p_dm->cut_version ==  ODM_CUT_A) ? 15 : p_dm->cut_version;
	u8	pkg_type_for_para = (p_dm->package_type == 0) ? 15 : p_dm->package_type;

	u32	driver1 = cut_version_for_para << 24 |
			(p_dm->support_interface & 0xF0) << 16 |
			p_dm->support_platform << 16 |
			pkg_type_for_para << 12 |
			(p_dm->support_interface & 0x0F) << 8  |
			_board_type;

	u32	driver2 = (p_dm->type_glna & 0xFF) <<  0 |
			(p_dm->type_gpa & 0xFF)  <<  8 |
			(p_dm->type_alna & 0xFF) << 16 |
			(p_dm->type_apa & 0xFF)  << 24;

	u32	driver3 = 0;

	u32	driver4 = (p_dm->type_glna & 0xFF00) >>  8 |
			(p_dm->type_gpa & 0xFF00) |
			(p_dm->type_alna & 0xFF00) << 8 |
			(p_dm->type_apa & 0xFF00)  << 16;

	PHYDM_DBG(p_dm, ODM_COMP_INIT,
	("===> check_positive (cond1, cond2, cond3, cond4) = (0x%X 0x%X 0x%X 0x%X)\n", cond1, cond2, cond3, cond4));
	PHYDM_DBG(p_dm, ODM_COMP_INIT,
	("===> check_positive (driver1, driver2, driver3, driver4) = (0x%X 0x%X 0x%X 0x%X)\n", driver1, driver2, driver3, driver4));

	PHYDM_DBG(p_dm, ODM_COMP_INIT,
	("	(Platform, Interface) = (0x%X, 0x%X)\n", p_dm->support_platform, p_dm->support_interface));
	PHYDM_DBG(p_dm, ODM_COMP_INIT,
	("	(Board, Package) = (0x%X, 0x%X)\n", p_dm->board_type, p_dm->package_type));


	/*============== value Defined Check ===============*/
	/*QFN type [15:12] and cut version [27:24] need to do value check*/

	if (((cond1 & 0x0000F000) != 0) && ((cond1 & 0x0000F000) != (driver1 & 0x0000F000)))
		return false;
	if (((cond1 & 0x0F000000) != 0) && ((cond1 & 0x0F000000) != (driver1 & 0x0F000000)))
		return false;

	/*=============== Bit Defined Check ================*/
	/* We don't care [31:28] */

	cond1 &= 0x00FF0FFF;
	driver1 &= 0x00FF0FFF;

	if ((cond1 & driver1) == cond1) {
		u32	bit_mask = 0;

		if ((cond1 & 0x0F) == 0) /* board_type is DONTCARE*/
			return true;

		if ((cond1 & BIT(0)) != 0) /*GLNA*/
			bit_mask |= 0x000000FF;
		if ((cond1 & BIT(1)) != 0) /*GPA*/
			bit_mask |= 0x0000FF00;
		if ((cond1 & BIT(2)) != 0) /*ALNA*/
			bit_mask |= 0x00FF0000;
		if ((cond1 & BIT(3)) != 0) /*APA*/
			bit_mask |= 0xFF000000;

		if (((cond2 & bit_mask) == (driver2 & bit_mask)) && ((cond4 & bit_mask) == (driver4 & bit_mask)))  /* board_type of each RF path is matched*/
			return true;
		else
			return false;
	} else
		return false;
}

/******************************************************************************
*                           agc_tab.TXT
******************************************************************************/

static u32 array_mp_8723d_agc_tab[] = {
		0xC78, 0xFE000101,
		0xC78, 0xFD010101,
		0xC78, 0xFC020101,
		0xC78, 0xFB030101,
		0xC78, 0xFA040101,
		0xC78, 0xF9050101,
		0xC78, 0xF8060101,
		0xC78, 0xF7070101,
		0xC78, 0xF6080101,
		0xC78, 0xF5090101,
		0xC78, 0xF40A0101,
		0xC78, 0xF30B0101,
		0xC78, 0xF20C0101,
		0xC78, 0xF10D0101,
		0xC78, 0xF00E0101,
		0xC78, 0xEF0F0101,
		0xC78, 0xEE100101,
		0xC78, 0xED110101,
		0xC78, 0xEC120101,
		0xC78, 0xEB130101,
		0xC78, 0xEA140101,
		0xC78, 0xE9150101,
		0xC78, 0xE8160101,
		0xC78, 0xE7170101,
		0xC78, 0xE6180101,
		0xC78, 0xE5190101,
		0xC78, 0xE41A0101,
		0xC78, 0xE31B0101,
		0xC78, 0xE21C0101,
		0xC78, 0xE11D0101,
		0xC78, 0xE01E0101,
		0xC78, 0x861F0101,
		0xC78, 0x85200101,
		0xC78, 0x84210101,
		0xC78, 0x83220101,
		0xC78, 0x82230101,
		0xC78, 0x81240101,
		0xC78, 0x80250101,
		0xC78, 0x44260101,
		0xC78, 0x43270101,
		0xC78, 0x42280101,
		0xC78, 0x41290101,
		0xC78, 0x402A0101,
		0xC78, 0x022B0101,
		0xC78, 0x012C0101,
		0xC78, 0x002D0101,
		0xC78, 0xC52E0001,
		0xC78, 0xC42F0001,
		0xC78, 0xC3300001,
		0xC78, 0xC2310001,
		0xC78, 0xC1320001,
		0xC78, 0xC0330001,
		0xC78, 0x04340001,
		0xC78, 0x03350001,
		0xC78, 0x02360001,
		0xC78, 0x01370001,
		0xC78, 0x00380001,
		0xC78, 0x00390001,
		0xC78, 0x003A0001,
		0xC78, 0x003B0001,
		0xC78, 0x003C0001,
		0xC78, 0x003D0001,
		0xC78, 0x003E0001,
		0xC78, 0x003F0001,
		0xC78, 0x6F002001,
		0xC78, 0x6F012001,
		0xC78, 0x6F022001,
		0xC78, 0x6F032001,
		0xC78, 0x6F042001,
		0xC78, 0x6F052001,
		0xC78, 0x6F062001,
		0xC78, 0x6F072001,
		0xC78, 0x6F082001,
		0xC78, 0x6F092001,
		0xC78, 0x6F0A2001,
		0xC78, 0x6F0B2001,
		0xC78, 0x6F0C2001,
		0xC78, 0x6F0D2001,
		0xC78, 0x6F0E2001,
		0xC78, 0x6F0F2001,
		0xC78, 0x6F102001,
		0xC78, 0x6F112001,
		0xC78, 0x6F122001,
		0xC78, 0x6F132001,
		0xC78, 0x6F142001,
		0xC78, 0x6F152001,
		0xC78, 0x6F162001,
		0xC78, 0x6F172001,
		0xC78, 0x6F182001,
		0xC78, 0x6F192001,
		0xC78, 0x6F1A2001,
		0xC78, 0x6F1B2001,
		0xC78, 0x6F1C2001,
		0xC78, 0x6F1D2001,
		0xC78, 0x6F1E2001,
		0xC78, 0x6F1F2001,
		0xC78, 0x6F202001,
		0xC78, 0x6F212001,
		0xC78, 0x6F222001,
		0xC78, 0x6F232001,
		0xC78, 0x6E242001,
		0xC78, 0x6D252001,
		0xC78, 0x6C262001,
		0xC78, 0x6B272001,
		0xC78, 0x6A282001,
		0xC78, 0x69292001,
		0xC78, 0x4B2A2001,
		0xC78, 0x4A2B2001,
		0xC78, 0x492C2001,
		0xC78, 0x482D2001,
		0xC78, 0x472E2001,
		0xC78, 0x462F2001,
		0xC78, 0x45302001,
		0xC78, 0x44312001,
		0xC78, 0x43322001,
		0xC78, 0x42332001,
		0xC78, 0x41342001,
		0xC78, 0x40352001,
		0xC78, 0x02362001,
		0xC78, 0x01372001,
		0xC78, 0x00382001,
		0xC78, 0x00392001,
		0xC78, 0x003A2001,
		0xC78, 0x003B2001,
		0xC78, 0x003C2001,
		0xC78, 0x003D2001,
		0xC78, 0x003E2001,
		0xC78, 0x003F2001,
		0xC78, 0x7F003101,
		0xC78, 0x7F013101,
		0xC78, 0x7F023101,
		0xC78, 0x7F033101,
		0xC78, 0x7F043101,
		0xC78, 0x7F053101,
		0xC78, 0x7F063101,
		0xC78, 0x7F073101,
		0xC78, 0x7E083101,
		0xC78, 0x7D093101,
		0xC78, 0x7C0A3101,
		0xC78, 0x7B0B3101,
		0xC78, 0x7A0C3101,
		0xC78, 0x790D3101,
		0xC78, 0x780E3101,
		0xC78, 0x770F3101,
		0xC78, 0x76103101,
		0xC78, 0x75113101,
		0xC78, 0x74123101,
		0xC78, 0x73133101,
		0xC78, 0x72143101,
		0xC78, 0x71153101,
		0xC78, 0x70163101,
		0xC78, 0x6F173101,
		0xC78, 0x6E183101,
		0xC78, 0x6D193101,
		0xC78, 0x6C1A3101,
		0xC78, 0x6B1B3101,
		0xC78, 0x6A1C3101,
		0xC78, 0x691D3101,
		0xC78, 0x681E3101,
		0xC78, 0x4B1F3101,
		0xC78, 0x4A203101,
		0xC78, 0x49213101,
		0xC78, 0x48223101,
		0xC78, 0x47233101,
		0xC78, 0x46243101,
		0xC78, 0x45253101,
		0xC78, 0x44263101,
		0xC78, 0x43273101,
		0xC78, 0x42283101,
		0xC78, 0x41293101,
		0xC78, 0x402A3101,
		0xC78, 0x022B3101,
		0xC78, 0x012C3101,
		0xC78, 0x002D3101,
		0xC78, 0x002E3101,
		0xC78, 0x002F3101,
		0xC78, 0x00303101,
		0xC78, 0x00313101,
		0xC78, 0x00323101,
		0xC78, 0x00333101,
		0xC78, 0x00343101,
		0xC78, 0x00353101,
		0xC78, 0x00363101,
		0xC78, 0x00373101,
		0xC78, 0x00383101,
		0xC78, 0x00393101,
		0xC78, 0x003A3101,
		0xC78, 0x003B3101,
		0xC78, 0x003C3101,
		0xC78, 0x003D3101,
		0xC78, 0x003E3101,
		0xC78, 0x003F3101,
		0xC78, 0xFE403101,
		0xC78, 0xFD413101,
		0xC78, 0xFC423101,
		0xC78, 0xFB433101,
		0xC78, 0xFA443101,
		0xC78, 0xF9453101,
		0xC78, 0xF8463101,
		0xC78, 0xF7473101,
		0xC78, 0xF6483101,
		0xC78, 0xF5493101,
		0xC78, 0xF44A3101,
		0xC78, 0xF34B3101,
		0xC78, 0xF24C3101,
		0xC78, 0xF14D3101,
		0xC78, 0xF04E3101,
		0xC78, 0xEF4F3101,
		0xC78, 0xEE503101,
		0xC78, 0xED513101,
		0xC78, 0xEC523101,
		0xC78, 0xEB533101,
		0xC78, 0xEA543101,
		0xC78, 0xE9553101,
		0xC78, 0xE8563101,
		0xC78, 0xE7573101,
		0xC78, 0xE6583101,
		0xC78, 0xE5593101,
		0xC78, 0xE45A3101,
		0xC78, 0xE35B3101,
		0xC78, 0xE25C3101,
		0xC78, 0xE15D3101,
		0xC78, 0xE05E3101,
		0xC78, 0x865F3101,
		0xC78, 0x85603101,
		0xC78, 0x84613101,
		0xC78, 0x83623101,
		0xC78, 0x82633101,
		0xC78, 0x81643101,
		0xC78, 0x80653101,
		0xC78, 0x80663101,
		0xC78, 0x80673101,
		0xC78, 0x80683101,
		0xC78, 0x80693101,
		0xC78, 0x806A3101,
		0xC78, 0x806B3101,
		0xC78, 0x806C3101,
		0xC78, 0x806D3101,
		0xC78, 0x806E3101,
		0xC78, 0x806F3101,
		0xC78, 0x80703101,
		0xC78, 0x80713101,
		0xC78, 0x80723101,
		0xC78, 0x80733101,
		0xC78, 0x80743101,
		0xC78, 0x80753101,
		0xC78, 0x80763101,
		0xC78, 0x80773101,
		0xC78, 0x80783101,
		0xC78, 0x80793101,
		0xC78, 0x807A3101,
		0xC78, 0x807B3101,
		0xC78, 0x807C3101,
		0xC78, 0x807D3101,
		0xC78, 0x807E3101,
		0xC78, 0x807F3101,
		0xC78, 0xEF402001,
		0xC78, 0xEF412001,
		0xC78, 0xEF422001,
		0xC78, 0xEF432001,
		0xC78, 0xEF442001,
		0xC78, 0xEF452001,
		0xC78, 0xEF462001,
		0xC78, 0xEF472001,
		0xC78, 0xEF482001,
		0xC78, 0xEF492001,
		0xC78, 0xEF4A2001,
		0xC78, 0xEF4B2001,
		0xC78, 0xEF4C2001,
		0xC78, 0xEF4D2001,
		0xC78, 0xEF4E2001,
		0xC78, 0xEF4F2001,
		0xC78, 0xEF502001,
		0xC78, 0xEF512001,
		0xC78, 0xEF522001,
		0xC78, 0xEF532001,
		0xC78, 0xEF542001,
		0xC78, 0xEF552001,
		0xC78, 0xEF562001,
		0xC78, 0xEF572001,
		0xC78, 0xEF582001,
		0xC78, 0xEF592001,
		0xC78, 0xEF5A2001,
		0xC78, 0xEF5B2001,
		0xC78, 0xEF5C2001,
		0xC78, 0xEF5D2001,
		0xC78, 0xEF5E2001,
		0xC78, 0xEF5F2001,
		0xC78, 0xEF602001,
		0xC78, 0xEE612001,
		0xC78, 0xED622001,
		0xC78, 0xEC632001,
		0xC78, 0xEB642001,
		0xC78, 0xEA652001,
		0xC78, 0xE9662001,
		0xC78, 0xE8672001,
		0xC78, 0xCB682001,
		0xC78, 0xCA692001,
		0xC78, 0xC96A2001,
		0xC78, 0xC86B2001,
		0xC78, 0xC76C2001,
		0xC78, 0xC66D2001,
		0xC78, 0xC56E2001,
		0xC78, 0xC46F2001,
		0xC78, 0xC3702001,
		0xC78, 0xC2712001,
		0xC78, 0xC1722001,
		0xC78, 0xC0732001,
		0xC78, 0x82742001,
		0xC78, 0x81752001,
		0xC78, 0x80762001,
		0xC78, 0x80772001,
		0xC78, 0x80782001,
		0xC78, 0x80792001,
		0xC78, 0x807A2001,
		0xC78, 0x807B2001,
		0xC78, 0x807C2001,
		0xC78, 0x807D2001,
		0xC78, 0x807E2001,
		0xC78, 0x807F2001,
		0xC78, 0xFA001101,
		0xC78, 0xF9011101,
		0xC78, 0xF8021101,
		0xC78, 0xF7031101,
		0xC78, 0xF6041101,
		0xC78, 0xF5051101,
		0xC78, 0xF4061101,
		0xC78, 0xD7071101,
		0xC78, 0xD6081101,
		0xC78, 0xD5091101,
		0xC78, 0xD40A1101,
		0xC78, 0x970B1101,
		0xC78, 0x960C1101,
		0xC78, 0x950D1101,
		0xC78, 0x940E1101,
		0xC78, 0x930F1101,
		0xC78, 0x92101101,
		0xC78, 0x91111101,
		0xC78, 0x90121101,
		0xC78, 0x8F131101,
		0xC78, 0x8E141101,
		0xC78, 0x8D151101,
		0xC78, 0x8C161101,
		0xC78, 0x8B171101,
		0xC78, 0x8A181101,
		0xC78, 0x89191101,
		0xC78, 0x881A1101,
		0xC78, 0x871B1101,
		0xC78, 0x861C1101,
		0xC78, 0x851D1101,
		0xC78, 0x841E1101,
		0xC78, 0x831F1101,
		0xC78, 0x82201101,
		0xC78, 0x81211101,
		0xC78, 0x80221101,
		0xC78, 0x43231101,
		0xC78, 0x42241101,
		0xC78, 0x41251101,
		0xC78, 0x04261101,
		0xC78, 0x03271101,
		0xC78, 0x02281101,
		0xC78, 0x01291101,
		0xC78, 0x002A1101,
		0xC78, 0xC42B1001,
		0xC78, 0xC32C1001,
		0xC78, 0xC22D1001,
		0xC78, 0xC12E1001,
		0xC78, 0xC02F1001,
		0xC78, 0x85301001,
		0xC78, 0x84311001,
		0xC78, 0x83321001,
		0xC78, 0x82331001,
		0xC78, 0x81341001,
		0xC78, 0x80351001,
		0xC78, 0x05361001,
		0xC78, 0x04371001,
		0xC78, 0x03381001,
		0xC78, 0x02391001,
		0xC78, 0x013A1001,
		0xC78, 0x003B1001,
		0xC78, 0x003C1001,
		0xC78, 0x003D1001,
		0xC78, 0x003E1001,
		0xC78, 0x003F1001,
		0xC50, 0x69553422,
		0xC50, 0x69553420,

};

void
odm_read_and_config_mp_8723d_agc_tab(
	struct	PHY_DM_STRUCT *p_dm
)
{
	u32	i = 0;
	u8	c_cond;
	bool	is_matched = true, is_skipped = false;
	u32	array_len = sizeof(array_mp_8723d_agc_tab)/sizeof(u32);
	u32	*array = array_mp_8723d_agc_tab;

	u32	v1 = 0, v2 = 0, pre_v1 = 0, pre_v2 = 0;

	PHYDM_DBG(p_dm, ODM_COMP_INIT, ("===> odm_read_and_config_mp_8723d_agc_tab\n"));

	while ((i + 1) < array_len) {
		v1 = array[i];
		v2 = array[i + 1];

		if (v1 & (BIT(31) | BIT(30))) {/*positive & negative condition*/
			if (v1 & BIT(31)) {/* positive condition*/
				c_cond  = (u8)((v1 & (BIT(29)|BIT(28))) >> 28);
				if (c_cond == COND_ENDIF) {/*end*/
					is_matched = true;
					is_skipped = false;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("ENDIF\n"));
				} else if (c_cond == COND_ELSE) { /*else*/
					is_matched = is_skipped?false:true;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("ELSE\n"));
				} else {/*if , else if*/
					pre_v1 = v1;
					pre_v2 = v2;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("IF or ELSE IF\n"));
				}
			} else if (v1 & BIT(30)) { /*negative condition*/
				if (!is_skipped) {
					if (check_positive(p_dm, pre_v1, pre_v2, v1, v2)) {
						is_matched = true;
						is_skipped = true;
					} else {
						is_matched = false;
						is_skipped = false;
					}
				} else
					is_matched = false;
			}
		} else {
			if (is_matched)
				odm_config_bb_agc_8723d(p_dm, v1, MASKDWORD, v2);
		}
		i = i + 2;
	}
}

u32
odm_get_version_mp_8723d_agc_tab(void)
{
		return 37;
}

/******************************************************************************
*                           phy_reg.TXT
******************************************************************************/

static u32 array_mp_8723d_phy_reg[] = {
		0x800, 0x80046C00,
		0x804, 0x00000003,
		0x808, 0x0000FC00,
		0x80C, 0x0000000A,
		0x810, 0x10001331,
		0x814, 0x020C3D10,
		0x818, 0x00200385,
		0x81C, 0x00000000,
		0x820, 0x01000100,
		0x824, 0x00390204,
		0x828, 0x00000000,
		0x82C, 0x00000000,
		0x830, 0x00000000,
		0x834, 0x00000000,
		0x838, 0x00000000,
		0x83C, 0x00000000,
		0x840, 0x00010000,
		0x844, 0x00000000,
		0x848, 0x00000000,
		0x84C, 0x00000000,
		0x850, 0x00000000,
		0x854, 0x00000000,
		0x858, 0x569A11A9,
		0x85C, 0x01000014,
		0x860, 0x66F60110,
		0x864, 0x461F0641,
		0x868, 0x00000000,
		0x86C, 0x27272700,
		0x870, 0x07000460,
		0x874, 0x25004000,
		0x878, 0x00000808,
		0x87C, 0x004F0201,
		0x880, 0xB2002E12,
		0x884, 0x00000007,
		0x888, 0x00000000,
		0x88C, 0xCCC000C0,
		0x890, 0x00000800,
		0x894, 0xFFFFFFFE,
		0x898, 0x40302010,
		0x89C, 0x00706050,
		0x900, 0x00000000,
		0x904, 0x00000023,
		0x908, 0x00000000,
		0x90C, 0x81121111,
		0x910, 0x00000402,
		0x914, 0x00000300,
		0x920, 0x18C6318C,
		0x924, 0x0000018C,
		0x948, 0x99000000,
		0x94C, 0x00000010,
		0x950, 0x00003800,
		0x954, 0x5A380000,
		0x958, 0x4BC6D87A,
		0x95C, 0x04EB9B79,
		0x96C, 0x00000003,
		0x970, 0x00000000,
		0x974, 0x00000000,
		0x978, 0x00000000,
		0x97C, 0x13000000,
		0x980, 0x00000000,
		0xA00, 0x00D046C8,
		0xA04, 0x80FF800C,
		0xA08, 0x8C838300,
		0xA0C, 0x2E20100F,
		0xA10, 0x9500BB78,
		0xA14, 0x1114D028,
		0xA18, 0x00881117,
		0xA1C, 0x89140F00,
		0xA20, 0xE82C0001,
		0xA24, 0x64B80C1C,
		0xA28, 0x00008810,
		0xA2C, 0x00D30000,
		0xA70, 0x101FBF00,
		0xA74, 0x00000007,
		0xA78, 0x00008900,
		0xA7C, 0x225B0606,
		0xA80, 0x2180FA74,
		0xA84, 0x00200000,
		0xA88, 0x040C0000,
		0xA8C, 0x12345678,
		0xA90, 0xABCDEF00,
		0xA94, 0x001B1B89,
		0xA98, 0x00000000,
		0xA9C, 0x00020000,
		0xAA0, 0x00000000,
		0xAA4, 0x0000000C,
		0xAA8, 0xCA100008,
		0xAAC, 0x01235667,
		0xAB0, 0x00000000,
		0xAB4, 0x20201402,
		0xB2C, 0x00000000,
		0xC00, 0x48071D40,
		0xC04, 0x03A05611,
		0xC08, 0x000000E4,
		0xC0C, 0x6C6C6C6C,
		0xC10, 0x28800000,
		0xC14, 0x40000100,
		0xC18, 0x08800000,
		0xC1C, 0x40000100,
		0xC20, 0x00000000,
		0xC24, 0x00000000,
		0xC28, 0x00000000,
		0xC2C, 0x00000000,
		0xC30, 0x69E9AC48,
		0xC34, 0x31000040,
		0xC38, 0x21688080,
		0xC3C, 0x000016D4,
		0xC40, 0x1F78403F,
		0xC44, 0x00010036,
		0xC48, 0xEC020107,
		0xC4C, 0x007F037F,
		0xC50, 0x69553420,
		0xC54, 0x43BC0094,
		0xC58, 0x00015969,
		0xC5C, 0x00310492,
		0xC60, 0x00280A00,
		0xC64, 0x7112848B,
		0xC68, 0x47C074FF,
		0xC6C, 0x00000036,
		0xC70, 0x2C7F000D,
		0xC74, 0x020600DB,
		0xC78, 0x0000001F,
		0xC7C, 0x00B91612,
		0xC80, 0x390000E4,
		0xC84, 0x21F60000,
		0xC88, 0x40000100,
		0xC8C, 0x20200000,
		0xC90, 0x00091521,
		0xC94, 0x00000000,
		0xC98, 0x00121820,
		0xC9C, 0x00007F7F,
		0xCA0, 0x00012000,
		0xCA4, 0x800000A0,
		0xCA8, 0x84E6C606,
		0xCAC, 0x00000060,
		0xCB0, 0x00000000,
		0xCB4, 0x00000000,
		0xCB8, 0x00000000,
		0xCBC, 0x28000000,
		0xCC0, 0x0010A3D0,
		0xCC4, 0x00000F7D,
		0xCC8, 0x000442D6,
		0xCCC, 0x00000000,
		0xCD0, 0x000001C8,
		0xCD4, 0x001C8000,
		0xCD8, 0x00000100,
		0xCDC, 0x40100000,
		0xCE0, 0x00222220,
		0xCE4, 0x20000000,
		0xCE8, 0x37644302,
		0xCEC, 0x2F97D40C,
		0xD00, 0x00030740,
		0xD04, 0x40020401,
		0xD08, 0x0000907F,
		0xD0C, 0x20010201,
		0xD10, 0xA0633333,
		0xD14, 0x3333BC53,
		0xD18, 0x7A8F5B6F,
		0xD2C, 0xCC979975,
		0xD30, 0x00000000,
		0xD34, 0x40608000,
		0xD38, 0x88000000,
		0xD3C, 0xC0127343,
		0xD40, 0x00000000,
		0xD44, 0x00000000,
		0xD48, 0x00000000,
		0xD4C, 0x00000000,
		0xD50, 0x00000038,
		0xD54, 0x00000000,
		0xD58, 0x00000282,
		0xD5C, 0x30032064,
		0xD60, 0x4653DE68,
		0xD64, 0x04518A3C,
		0xD68, 0x00002101,
		0xE00, 0x2D2D2D2D,
		0xE04, 0x2D2D2D2D,
		0xE08, 0x0390272D,
		0xE10, 0x2D2D2D2D,
		0xE14, 0x2D2D2D2D,
		0xE18, 0x2D2D2D2D,
		0xE1C, 0x2D2D2D2D,
		0xE28, 0x00000000,
		0xE30, 0x1000DC1F,
		0xE34, 0x10008C1F,
		0xE38, 0x02140102,
		0xE3C, 0x681604C2,
		0xE40, 0x01007C00,
		0xE44, 0x01004800,
		0xE48, 0xFB000000,
		0xE4C, 0x000028D1,
		0xE50, 0x1000DC1F,
		0xE54, 0x10008C1F,
		0xE58, 0x02140102,
		0xE5C, 0x28160D05,
		0xE60, 0x00000008,
		0xE68, 0x001B25A4,
		0xE6C, 0x01C00014,
		0xE70, 0x01C00016,
		0xE74, 0x02000014,
		0xE78, 0x02000014,
		0xE7C, 0x02000014,
		0xE80, 0x02000014,
		0xE84, 0x01C00014,
		0xE88, 0x02000014,
		0xE8C, 0x01C00014,
		0xED0, 0x01C00014,
		0xED4, 0x01C00014,
		0xED8, 0x01C00014,
		0xEDC, 0x00000014,
		0xEE0, 0x00000014,
		0xEE8, 0x21555448,
		0xEEC, 0x03C00014,
		0xF14, 0x00000003,
		0xF00, 0x00100300,
		0xF08, 0x0000800B,
		0xF0C, 0x0000F007,
		0xF10, 0x0000A487,
		0xF1C, 0x80000064,
		0xF38, 0x00030155,
		0xF3C, 0x0000003A,
		0xF4C, 0x13000000,
		0xF50, 0x00000000,
		0xF18, 0x00000000,

};

void
odm_read_and_config_mp_8723d_phy_reg(
	struct	PHY_DM_STRUCT *p_dm
)
{
	u32	i = 0;
	u8	c_cond;
	bool	is_matched = true, is_skipped = false;
	u32	array_len = sizeof(array_mp_8723d_phy_reg)/sizeof(u32);
	u32	*array = array_mp_8723d_phy_reg;

	u32	v1 = 0, v2 = 0, pre_v1 = 0, pre_v2 = 0;

	PHYDM_DBG(p_dm, ODM_COMP_INIT, ("===> odm_read_and_config_mp_8723d_phy_reg\n"));

	while ((i + 1) < array_len) {
		v1 = array[i];
		v2 = array[i + 1];

		if (v1 & (BIT(31) | BIT(30))) {/*positive & negative condition*/
			if (v1 & BIT(31)) {/* positive condition*/
				c_cond  = (u8)((v1 & (BIT(29)|BIT(28))) >> 28);
				if (c_cond == COND_ENDIF) {/*end*/
					is_matched = true;
					is_skipped = false;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("ENDIF\n"));
				} else if (c_cond == COND_ELSE) { /*else*/
					is_matched = is_skipped?false:true;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("ELSE\n"));
				} else {/*if , else if*/
					pre_v1 = v1;
					pre_v2 = v2;
					PHYDM_DBG(p_dm, ODM_COMP_INIT, ("IF or ELSE IF\n"));
				}
			} else if (v1 & BIT(30)) { /*negative condition*/
				if (!is_skipped) {
					if (check_positive(p_dm, pre_v1, pre_v2, v1, v2)) {
						is_matched = true;
						is_skipped = true;
					} else {
						is_matched = false;
						is_skipped = false;
					}
				} else
					is_matched = false;
			}
		} else {
			if (is_matched)
				odm_config_bb_phy_8723d(p_dm, v1, MASKDWORD, v2);
		}
		i = i + 2;
	}
}

u32
odm_get_version_mp_8723d_phy_reg(void)
{
		return 37;
}

/******************************************************************************
*                           phy_reg_pg.TXT
******************************************************************************/

static u32 array_mp_8723d_phy_reg_pg[] = {
	0, 0, 0, 0x00000e08, 0x0000ff00, 0x00003200,
	0, 0, 0, 0x0000086c, 0xffffff00, 0x32323200,
	0, 0, 0, 0x00000e00, 0xffffffff, 0x32343434,
	0, 0, 0, 0x00000e04, 0xffffffff, 0x28303032,
	0, 0, 0, 0x00000e10, 0xffffffff, 0x30323234,
	0, 0, 0, 0x00000e14, 0xffffffff, 0x26282830
};

void
odm_read_and_config_mp_8723d_phy_reg_pg(
	struct PHY_DM_STRUCT	*p_dm
)
{
	u32	i = 0;
	u32	array_len = sizeof(array_mp_8723d_phy_reg_pg)/sizeof(u32);
	u32	*array = array_mp_8723d_phy_reg_pg;

	PHYDM_DBG(p_dm, ODM_COMP_INIT, ("===> odm_read_and_config_mp_8723d_phy_reg_pg\n"));

	p_dm->phy_reg_pg_version = 1;
	p_dm->phy_reg_pg_value_type = PHY_REG_PG_EXACT_VALUE;

	for (i = 0; i < array_len; i += 6) {
		u32	v1 = array[i];
		u32	v2 = array[i+1];
		u32	v3 = array[i+2];
		u32	v4 = array[i+3];
		u32	v5 = array[i+4];
		u32	v6 = array[i+5];

		odm_config_bb_phy_reg_pg_8723d(p_dm, v1, v2, v3, v4, v5, v6);
	}
}
