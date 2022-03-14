/* vi:set ts=8 sts=4 sw=4 noet:
 *
 * VIM - Vi IMproved	by Bram Moolenaar
 *
 * Do ":help uganda"  in Vim to read copying and usage conditions.
 * Do ":help credits" in Vim to see a list of people who contributed.
 * See README.txt for an overview of the Vim source code.
 */

/*
 * drawline.c: Functions for drawing window lines on the screen.
 * This is the middle level, drawscreen. is the higher level and screen.c the
 * lower level.
 */

#include "vim.h"

#ifdef FEAT_SYN_HL
/*
 * Advance **color_cols and return TRUE when there are columns to draw.
 */
    static int
advance_color_col(int vcol, int **color_cols)
{
    while (**color_cols >= 0 && vcol > **color_cols)
	++*color_cols;
    return (**color_cols >= 0);
}
#endif

#ifdef FEAT_SYN_HL
/*
 * Used when 'cursorlineopt' contains "screenline": compute the margins between
 * which the highlighting is used.
 */
    static void
margin_columns_win(win_T *wp, int *left_col, int *right_col)
{
    // cache previous calculations depending on w_virtcol
    static int saved_w_virtcol;
    static win_T *prev_wp;
    static int prev_left_col;
    static int prev_right_col;
    static int prev_col_off;

    int cur_col_off = win_col_off(wp);
    int	width1;
    int	width2;

    if (saved_w_virtcol == wp->w_virtcol
	    && prev_wp == wp && prev_col_off == cur_col_off)
    {
	*right_col = prev_right_col;
	*left_col = prev_left_col;
	return;
    }

    width1 = wp->w_width - cur_col_off;
    width2 = width1 + win_col_off2(wp);

    *left_col = 0;
    *right_col = width1;

    if (wp->w_virtcol >= (colnr_T)width1)
	*right_col = width1 + ((wp->w_virtcol - width1) / width2 + 1) * width2;
    if (wp->w_virtcol >= (colnr_T)width1 && width2 > 0)
	*left_col = (wp->w_virtcol - width1) / width2 * width2 + width1;

    // cache values
    prev_left_col = *left_col;
    prev_right_col = *right_col;
    prev_wp = wp;
    saved_w_virtcol = wp->w_virtcol;
    prev_col_off = cur_col_off;
}
#endif

#ifdef FEAT_SIGNS
/*
 * Get information needed to display the sign in line 'lnum' in window 'wp'.
 * If 'nrcol' is TRUE, the sign is going to be displayed in the number column.
 * Otherwise the sign is going to be displayed in the sign column.
 */
    static void
get_sign_display_info(
	int		nrcol,
	win_T		*wp,
	linenr_T	lnum UNUSED,
	sign_attrs_T	*sattr,
	int		wcr_attr,
	int		row,
	int		startrow,
	int		filler_lines UNUSED,
	int		filler_todo UNUSED,
	int		*c_extrap,
	int		*c_finalp,
	char_u		*extra,
	char_u		**pp_extra,
	int		*n_extrap,
	int		*char_attrp)
{
    int	text_sign;
# ifdef FEAT_SIGN_ICONS
    int	icon_sign;
# endif

    // Draw two cells with the sign value or blank.
    *c_extrap = ' ';
    *c_finalp = NUL;
    if (nrcol)
	*n_extrap = number_width(wp) + 1;
    else
    {
	*char_attrp = hl_combine_attr(wcr_attr, HL_ATTR(HLF_SC));
	*n_extrap = 2;
    }

    if (row == startrow
#ifdef FEAT_DIFF
	    + filler_lines && filler_todo <= 0
#endif
       )
    {
	text_sign = (sattr->sat_text != NULL) ? sattr->sat_typenr : 0;
# ifdef FEAT_SIGN_ICONS
	icon_sign = (sattr->sat_icon != NULL) ? sattr->sat_typenr : 0;
	if (gui.in_use && icon_sign != 0)
	{
	    // Use the image in this position.
	    if (nrcol)
	    {
		*c_extrap = NUL;
		sprintf((char *)extra, "%-*c ", number_width(wp), SIGN_BYTE);
		*pp_extra = extra;
		*n_extrap = (int)STRLEN(*pp_extra);
	    }
	    else
		*c_extrap = SIGN_BYTE;
#  ifdef FEAT_NETBEANS_INTG
	    if (netbeans_active() && (buf_signcount(wp->w_buffer, lnum) > 1))
	    {
		if (nrcol)
		{
		    *c_extrap = NUL;
		    sprintf((char *)extra, "%-*c ", number_width(wp),
							MULTISIGN_BYTE);
		    *pp_extra = extra;
		    *n_extrap = (int)STRLEN(*pp_extra);
		}
		else
		    *c_extrap = MULTISIGN_BYTE;
	    }
#  endif
	    *c_finalp = NUL;
	    *char_attrp = icon_sign;
	}
	else
# endif
	    if (text_sign != 0)
	    {
		*pp_extra = sattr->sat_text;
		if (*pp_extra != NULL)
		{
		    if (nrcol)
		    {
			int n, width = number_width(wp) - 2;

			for (n = 0; n < width; n++)
			    extra[n] = ' ';
			extra[n] = 0;
			STRCAT(extra, *pp_extra);
			STRCAT(extra, " ");
			*pp_extra = extra;
		    }
		    *c_extrap = NUL;
		    *c_finalp = NUL;
		    *n_extrap = (int)STRLEN(*pp_extra);
		}
		*char_attrp = sattr->sat_texthl;
	    }
    }
}
#endif

#ifdef FEAT_PROP_POPUP
static textprop_T	*current_text_props = NULL;
static buf_T		*current_buf = NULL;

    static int
text_prop_compare(const void *s1, const void *s2)
{
    int  idx1, idx2;
    proptype_T  *pt1, *pt2;
    colnr_T col1, col2;

    idx1 = *(int *)s1;
    idx2 = *(int *)s2;
    pt1 = text_prop_type_by_id(current_buf, current_text_props[idx1].tp_type);
    pt2 = text_prop_type_by_id(current_buf, current_text_props[idx2].tp_type);
    if (pt1 == pt2)
	return 0;
    if (pt1 == NULL)
	return -1;
    if (pt2 == NULL)
	return 1;
    if (pt1->pt_priority != pt2->pt_priority)
	return pt1->pt_priority > pt2->pt_priority ? 1 : -1;
    col1 = current_text_props[idx1].tp_col;
    col2 = current_text_props[idx2].tp_col;
    return col1 == col2 ? 0 : col1 > col2 ? 1 : -1;
}
#endif

/*
 * Display line "lnum" of window 'wp' on the screen.
 * Start at row "startrow", stop when "endrow" is reached.
 * wp->w_virtcol needs to be valid.
 *
 * Return the number of last row the line occupies.
 */
    int
win_line(
    win_T	*wp,
    linenr_T	lnum,
    int		startrow,
    int		endrow,
    int		nochange UNUSED,	// not updating for changed text
    int		number_only)		// only update the number column
{
    int		col = 0;		// visual column on screen
    unsigned	off;			// offset in ScreenLines/ScreenAttrs
    int		c = 0;			// init for GCC
    long	vcol = 0;		// virtual column (for tabs)
#ifdef FEAT_LINEBREAK
    long	vcol_sbr = -1;		// virtual column after showbreak
#endif
    long	vcol_prev = -1;		// "vcol" of previous character
    char_u	*line;			// current line
    char_u	*ptr;			// current position in "line"
    int		row;			// row in the window, excl w_winrow
    int		screen_row;		// row on the screen, incl w_winrow

    char_u	extra[21];		// "%ld " and 'fdc' must fit in here
    int		n_extra = 0;		// number of extra chars
    char_u	*p_extra = NULL;	// string of extra chars, plus NUL
    char_u	*p_extra_free = NULL;   // p_extra needs to be freed
    int		c_extra = NUL;		// extra chars, all the same
    int		c_final = NUL;		// final char, mandatory if set
    int		extra_attr = 0;		// attributes when n_extra != 0
    static char_u *at_end_str = (char_u *)""; // used for p_extra when
					   // displaying lcs_eol at end-of-line
    int		lcs_eol_one = lcs_eol;	// lcs_eol until it's been used
    int		lcs_prec_todo = lcs_prec;   // lcs_prec until it's been used

    // saved "extra" items for when draw_state becomes WL_LINE (again)
    int		saved_n_extra = 0;
    char_u	*saved_p_extra = NULL;
    int		saved_c_extra = 0;
    int		saved_c_final = 0;
    int		saved_char_attr = 0;

    int		n_attr = 0;		// chars with special attr
    int		saved_attr2 = 0;	// char_attr saved for n_attr
    int		n_attr3 = 0;		// chars with overruling special attr
    int		saved_attr3 = 0;	// char_attr saved for n_attr3

    int		n_skip = 0;		// nr of chars to skip for 'nowrap'

    int		fromcol = -10;		// start of inverting
    int		tocol = MAXCOL;		// end of inverting
    int		fromcol_prev = -2;	// start of inverting after cursor
    int		noinvcur = FALSE;	// don't invert the cursor
    int		lnum_in_visual_area = FALSE;
    pos_T	pos;
    long	v;

    int		char_attr = 0;		// attributes for next character
    int		attr_pri = FALSE;	// char_attr has priority
    int		area_highlighting = FALSE; // Visual or incsearch highlighting
					   // in this line
    int		vi_attr = 0;		// attributes for Visual and incsearch
					// highlighting
    int		wcr_attr = 0;		// attributes from 'wincolor'
    int		win_attr = 0;		// background for whole window, except
					// margins and "~" lines.
    int		area_attr = 0;		// attributes desired by highlighting
    int		search_attr = 0;	// attributes desired by 'hlsearch'
#ifdef FEAT_SYN_HL
    int		vcol_save_attr = 0;	// saved attr for 'cursorcolumn'
    int		syntax_attr = 0;	// attributes desired by syntax
    int		prev_syntax_col = -1;	// column of prev_syntax_attr
    int		prev_syntax_attr = 0;	// syntax_attr at prev_syntax_col
    int		has_syntax = FALSE;	// this buffer has syntax highl.
    int		save_did_emsg;
    int		draw_color_col = FALSE;	// highlight colorcolumn
    int		*color_cols = NULL;	// pointer to according columns array
#endif
    int		eol_hl_off = 0;		// 1 if highlighted char after EOL
#ifdef FEAT_PROP_POPUP
    int		text_prop_count;
    int		text_prop_next = 0;	// next text property to use
    textprop_T	*text_props = NULL;
    int		*text_prop_idxs = NULL;
    int		text_props_active = 0;
    proptype_T  *text_prop_type = NULL;
    int		text_prop_attr = 0;
    int		text_prop_combine = FALSE;
#endif
#ifdef FEAT_SPELL
    int		has_spell = FALSE;	// this buffer has spell checking
    int		can_spell;
# define SPWORDLEN 150
    char_u	nextline[SPWORDLEN * 2];// text with start of the next line
    int		nextlinecol = 0;	// column where nextline[] starts
    int		nextline_idx = 0;	// index in nextline[] where next line
					// starts
    int		spell_attr = 0;		// attributes desired by spelling
    int		word_end = 0;		// last byte with same spell_attr
    static linenr_T  checked_lnum = 0;	// line number for "checked_col"
    static int	checked_col = 0;	// column in "checked_lnum" up to which
					// there are no spell errors
    static int	cap_col = -1;		// column to check for Cap word
    static linenr_T capcol_lnum = 0;	// line number where "cap_col" used
    int		cur_checked_col = 0;	// checked column for current line
#endif
    int		extra_check = 0;	// has extra highlighting
    int		multi_attr = 0;		// attributes desired by multibyte
    int		mb_l = 1;		// multi-byte byte length
    int		mb_c = 0;		// decoded multi-byte character
    int		mb_utf8 = FALSE;	// screen char is UTF-8 char
    int		u8cc[MAX_MCO];		// composing UTF-8 chars
#if defined(FEAT_DIFF) || defined(FEAT_SIGNS)
    int		filler_lines = 0;	// nr of filler lines to be drawn
    int		filler_todo = 0;	// nr of filler lines still to do + 1
#endif
#ifdef FEAT_DIFF
    hlf_T	diff_hlf = (hlf_T)0;	// type of diff highlighting
    int		change_start = MAXCOL;	// first col of changed area
    int		change_end = -1;	// last col of changed area
#endif
    colnr_T	trailcol = MAXCOL;	// start of trailing spaces
#ifdef FEAT_LINEBREAK
    int		need_showbreak = FALSE; // overlong line, skipping first x
					// chars
#endif
#if defined(FEAT_SIGNS) || defined(FEAT_QUICKFIX) \
	|| defined(FEAT_SYN_HL) || defined(FEAT_DIFF)
# define LINE_ATTR
    int		line_attr = 0;		// attribute for the whole line
    int		line_attr_save;
#endif
#ifdef FEAT_SIGNS
    int		sign_present = FALSE;
    sign_attrs_T sattr;
#endif
#ifdef FEAT_ARABIC
    int		prev_c = 0;		// previous Arabic character
    int		prev_c1 = 0;		// first composing char for prev_c
#endif
#if defined(LINE_ATTR)
    int		did_line_attr = 0;
#endif
#ifdef FEAT_TERMINAL
    int		get_term_attr = FALSE;
#endif
#ifdef FEAT_SYN_HL
    int		cul_attr = 0;		// set when 'cursorline' active

    // 'cursorlineopt' has "screenline" and cursor is in this line
    int		cul_screenline = FALSE;

    // margin columns for the screen line, needed for when 'cursorlineopt'
    // contains "screenline"
    int		left_curline_col = 0;
    int		right_curline_col = 0;
#endif

    // draw_state: items that are drawn in sequence:
#define WL_START	0		// nothing done yet
#ifdef FEAT_CMDWIN
# define WL_CMDLINE	WL_START + 1	// cmdline window column
#else
# define WL_CMDLINE	WL_START
#endif
#ifdef FEAT_FOLDING
# define WL_FOLD	WL_CMDLINE + 1	// 'foldcolumn'
#else
# define WL_FOLD	WL_CMDLINE
#endif
#ifdef FEAT_SIGNS
# define WL_SIGN	WL_FOLD + 1	// column for signs
#else
# define WL_SIGN	WL_FOLD		// column for signs
#endif
#define WL_NR		WL_SIGN + 1	// line number
#ifdef FEAT_LINEBREAK
# define WL_BRI		WL_NR + 1	// 'breakindent'
#else
# define WL_BRI		WL_NR
#endif
#if defined(FEAT_LINEBREAK) || defined(FEAT_DIFF)
# define WL_SBR		WL_BRI + 1	// 'showbreak' or 'diff'
#else
# define WL_SBR		WL_BRI
#endif
#define WL_LINE		WL_SBR + 1	// text in the line
    int		draw_state = WL_START;	// what to draw next
#if defined(FEAT_XIM) && defined(FEAT_GUI_GTK)
    int		feedback_col = 0;
    int		feedback_old_attr = -1;
#endif
    int		screen_line_flags = 0;

#if defined(FEAT_CONCEAL) || defined(FEAT_SEARCH_EXTRA)
    int		match_conc	= 0;	// cchar for match functions
#endif
#ifdef FEAT_CONCEAL
    int		syntax_flags	= 0;
    int		syntax_seqnr	= 0;
    int		prev_syntax_id	= 0;
    int		conceal_attr	= HL_ATTR(HLF_CONCEAL);
    int		is_concealing	= FALSE;
    int		boguscols	= 0;	// nonexistent columns added to force
					// wrapping
    int		vcol_off	= 0;	// offset for concealed characters
    int		did_wcol	= FALSE;
    int		old_boguscols   = 0;
# define VCOL_HLC (vcol - vcol_off)
# define FIX_FOR_BOGUSCOLS \
    { \
	n_extra += vcol_off; \
	vcol -= vcol_off; \
	vcol_off = 0; \
	col -= boguscols; \
	old_boguscols = boguscols; \
	boguscols = 0; \
    }
#else
# define VCOL_HLC (vcol)
#endif

    if (startrow > endrow)		// past the end already!
	return startrow;

    row = startrow;
    screen_row = row + W_WINROW(wp);

    if (!number_only)
    {
	// To speed up the loop below, set extra_check when there is linebreak,
	// trailing white space and/or syntax processing to be done.
#ifdef FEAT_LINEBREAK
	extra_check = wp->w_p_lbr;
#endif
#ifdef FEAT_SYN_HL
	if (syntax_present(wp) && !wp->w_s->b_syn_error
# ifdef SYN_TIME_LIMIT
		&& !wp->w_s->b_syn_slow
# endif
	   )
	{
	    // Prepare for syntax highlighting in this line.  When there is an
	    // error, stop syntax highlighting.
	    save_did_emsg = did_emsg;
	    did_emsg = FALSE;
	    syntax_start(wp, lnum);
	    if (did_emsg)
		wp->w_s->b_syn_error = TRUE;
	    else
	    {
		did_emsg = save_did_emsg;
#ifdef SYN_TIME_LIMIT
		if (!wp->w_s->b_syn_slow)
#endif
		{
		    has_syntax = TRUE;
		    extra_check = TRUE;
		}
	    }
	}

	// Check for columns to display for 'colorcolumn'.
	color_cols = wp->w_p_cc_cols;
	if (color_cols != NULL)
	    draw_color_col = advance_color_col(VCOL_HLC, &color_cols);
#endif

#ifdef FEAT_TERMINAL
	if (term_show_buffer(wp->w_buffer))
	{
	    extra_check = TRUE;
	    get_term_attr = TRUE;
	    win_attr = term_get_attr(wp, lnum, -1);
	}
#endif

#ifdef FEAT_SPELL
	if (wp->w_p_spell
		&& *wp->w_s->b_p_spl != NUL
		&& wp->w_s->b_langp.ga_len > 0
		&& *(char **)(wp->w_s->b_langp.ga_data) != NULL)
	{
	    // Prepare for spell checking.
	    has_spell = TRUE;
	    extra_check = TRUE;

	    // Get the start of the next line, so that words that wrap to the
	    // next line are found too: "et<line-break>al.".
	    // Trick: skip a few chars for C/shell/Vim comments
	    nextline[SPWORDLEN] = NUL;
	    if (lnum < wp->w_buffer->b_ml.ml_line_count)
	    {
		line = ml_get_buf(wp->w_buffer, lnum + 1, FALSE);
		spell_cat_line(nextline + SPWORDLEN, line, SPWORDLEN);
	    }

	    // When a word wrapped from the previous line the start of the
	    // current line is valid.
	    if (lnum == checked_lnum)
		cur_checked_col = checked_col;
	    checked_lnum = 0;

	    // When there was a sentence end in the previous line may require a
	    // word starting with capital in this line.  In line 1 always check
	    // the first word.
	    if (lnum != capcol_lnum)
		cap_col = -1;
	    if (lnum == 1)
		cap_col = 0;
	    capcol_lnum = 0;
	}
#endif

	// handle Visual active in this window
	if (VIsual_active && wp->w_buffer == curwin->w_buffer)
	{
	    pos_T	*top, *bot;

	    if (LTOREQ_POS(curwin->w_cursor, VIsual))
	    {
		// Visual is after curwin->w_cursor
		top = &curwin->w_cursor;
		bot = &VIsual;
	    }
	    else
	    {
		// Visual is before curwin->w_cursor
		top = &VIsual;
		bot = &curwin->w_cursor;
	    }
	    lnum_in_visual_area = (lnum >= top->lnum && lnum <= bot->lnum);
	    if (VIsual_mode == Ctrl_V)
	    {
		// block mode
		if (lnum_in_visual_area)
		{
		    fromcol = wp->w_old_cursor_fcol;
		    tocol = wp->w_old_cursor_lcol;
		}
	    }
	    else
	    {
		// non-block mode
		if (lnum > top->lnum && lnum <= bot->lnum)
		    fromcol = 0;
		else if (lnum == top->lnum)
		{
		    if (VIsual_mode == 'V')	// linewise
			fromcol = 0;
		    else
		    {
			getvvcol(wp, top, (colnr_T *)&fromcol, NULL, NULL);
			if (gchar_pos(top) == NUL)
			    tocol = fromcol + 1;
		    }
		}
		if (VIsual_mode != 'V' && lnum == bot->lnum)
		{
		    if (*p_sel == 'e' && bot->col == 0 && bot->coladd == 0)
		    {
			fromcol = -10;
			tocol = MAXCOL;
		    }
		    else if (bot->col == MAXCOL)
			tocol = MAXCOL;
		    else
		    {
			pos = *bot;
			if (*p_sel == 'e')
			    getvvcol(wp, &pos, (colnr_T *)&tocol, NULL, NULL);
			else
			{
			    getvvcol(wp, &pos, NULL, NULL, (colnr_T *)&tocol);
			    ++tocol;
			}
		    }
		}
	    }

	    // Check if the character under the cursor should not be inverted
	    if (!highlight_match && lnum == curwin->w_cursor.lnum
								&& wp == curwin
#ifdef FEAT_GUI
		    && !gui.in_use
#endif
		    )
		noinvcur = TRUE;

	    // if inverting in this line set area_highlighting
	    if (fromcol >= 0)
	    {
		area_highlighting = TRUE;
		vi_attr = HL_ATTR(HLF_V);
#if defined(FEAT_CLIPBOARD) && defined(FEAT_X11)
		if ((clip_star.available && !clip_star.owned
						      && clip_isautosel_star())
			|| (clip_plus.available && !clip_plus.owned
						     && clip_isautosel_plus()))
		    vi_attr = HL_ATTR(HLF_VNC);
#endif
	    }
	}

	// handle 'incsearch' and ":s///c" highlighting
	else if (highlight_match
		&& wp == curwin
		&& lnum >= curwin->w_cursor.lnum
		&& lnum <= curwin->w_cursor.lnum + search_match_lines)
	{
	    if (lnum == curwin->w_cursor.lnum)
		getvcol(curwin, &(curwin->w_cursor),
					      (colnr_T *)&fromcol, NULL, NULL);
	    else
		fromcol = 0;
	    if (lnum == curwin->w_cursor.lnum + search_match_lines)
	    {
		pos.lnum = lnum;
		pos.col = search_match_endcol;
		getvcol(curwin, &pos, (colnr_T *)&tocol, NULL, NULL);
	    }
	    else
		tocol = MAXCOL;
	    // do at least one character; happens when past end of line
	    if (fromcol == tocol)
		tocol = fromcol + 1;
	    area_highlighting = TRUE;
	    vi_attr = HL_ATTR(HLF_I);
	}
    }

#ifdef FEAT_DIFF
    filler_lines = diff_check(wp, lnum);
    if (filler_lines < 0)
    {
	if (filler_lines == -1)
	{
	    if (diff_find_change(wp, lnum, &change_start, &change_end))
		diff_hlf = HLF_ADD;	// added line
	    else if (change_start == 0)
		diff_hlf = HLF_TXD;	// changed text
	    else
		diff_hlf = HLF_CHD;	// changed line
	}
	else
	    diff_hlf = HLF_ADD;		// added line
	filler_lines = 0;
	area_highlighting = TRUE;
    }
    if (lnum == wp->w_topline)
	filler_lines = wp->w_topfill;
    filler_todo = filler_lines;
#endif

#ifdef FEAT_SIGNS
    sign_present = buf_get_signattrs(wp, lnum, &sattr);
#endif

#ifdef LINE_ATTR
# ifdef FEAT_SIGNS
    // If this line has a sign with line highlighting set line_attr.
    if (sign_present)
	line_attr = sattr.sat_linehl;
# endif
# if defined(FEAT_QUICKFIX)
    // Highlight the current line in the quickfix window.
    if (bt_quickfix(wp->w_buffer) && qf_current_entry(wp) == lnum)
	line_attr = HL_ATTR(HLF_QFL);
# endif
    if (line_attr != 0)
	area_highlighting = TRUE;
#endif

    line = ml_get_buf(wp->w_buffer, lnum, FALSE);
    ptr = line;

#ifdef FEAT_SPELL
    if (has_spell && !number_only)
    {
	// For checking first word with a capital skip white space.
	if (cap_col == 0)
	    cap_col = getwhitecols(line);

	// To be able to spell-check over line boundaries copy the end of the
	// current line into nextline[].  Above the start of the next line was
	// copied to nextline[SPWORDLEN].
	if (nextline[SPWORDLEN] == NUL)
	{
	    // No next line or it is empty.
	    nextlinecol = MAXCOL;
	    nextline_idx = 0;
	}
	else
	{
	    v = (long)STRLEN(line);
	    if (v < SPWORDLEN)
	    {
		// Short line, use it completely and append the start of the
		// next line.
		nextlinecol = 0;
		mch_memmove(nextline, line, (size_t)v);
		STRMOVE(nextline + v, nextline + SPWORDLEN);
		nextline_idx = v + 1;
	    }
	    else
	    {
		// Long line, use only the last SPWORDLEN bytes.
		nextlinecol = v - SPWORDLEN;
		mch_memmove(nextline, line + nextlinecol, SPWORDLEN);
		nextline_idx = SPWORDLEN + 1;
	    }
	}
    }
#endif

    if (wp->w_p_list)
    {
	if (lcs_space || lcs_trail || lcs_nbsp)
	    extra_check = TRUE;
	// find start of trailing whitespace
	if (lcs_trail)
	{
	    trailcol = (colnr_T)STRLEN(ptr);
	    while (trailcol > (colnr_T)0 && VIM_ISWHITE(ptr[trailcol - 1]))
		--trailcol;
	    trailcol += (colnr_T) (ptr - line);
	}
    }

    wcr_attr = get_wcr_attr(wp);
    if (wcr_attr != 0)
    {
	win_attr = wcr_attr;
	area_highlighting = TRUE;
    }

#ifdef FEAT_PROP_POPUP
    if (WIN_IS_POPUP(wp))
	screen_line_flags |= SLF_POPUP;
#endif

    // 'nowrap' or 'wrap' and a single line that doesn't fit: Advance to the
    // first character to be displayed.
    if (wp->w_p_wrap)
	v = wp->w_skipcol;
    else
	v = wp->w_leftcol;
    if (v > 0 && !number_only)
    {
	char_u	*prev_ptr = ptr;

	while (vcol < v && *ptr != NUL)
	{
	    c = win_lbr_chartabsize(wp, line, ptr, (colnr_T)vcol, NULL);
	    vcol += c;
	    prev_ptr = ptr;
	    MB_PTR_ADV(ptr);
	}

	// When:
	// - 'cuc' is set, or
	// - 'colorcolumn' is set, or
	// - 'virtualedit' is set, or
	// - the visual mode is active,
	// the end of the line may be before the start of the displayed part.
	if (vcol < v && (
#ifdef FEAT_SYN_HL
	     wp->w_p_cuc || draw_color_col ||
#endif
	     virtual_active() ||
	     (VIsual_active && wp->w_buffer == curwin->w_buffer)))
	    vcol = v;

	// Handle a character that's not completely on the screen: Put ptr at
	// that character but skip the first few screen characters.
	if (vcol > v)
	{
	    vcol -= c;
	    ptr = prev_ptr;
	    // If the character fits on the screen, don't need to skip it.
	    // Except for a TAB.
	    if (( (*mb_ptr2cells)(ptr) >= c || *ptr == TAB) && col == 0)
	       n_skip = v - vcol;
	}

	// Adjust for when the inverted text is before the screen,
	// and when the start of the inverted text is before the screen.
	if (tocol <= vcol)
	    fromcol = 0;
	else if (fromcol >= 0 && fromcol < vcol)
	    fromcol = vcol;

#ifdef FEAT_LINEBREAK
	// When w_skipcol is non-zero, first line needs 'showbreak'
	if (wp->w_p_wrap)
	    need_showbreak = TRUE;
#endif
#ifdef FEAT_SPELL
	// When spell checking a word we need to figure out the start of the
	// word and if it's badly spelled or not.
	if (has_spell)
	{
	    int		len;
	    colnr_T	linecol = (colnr_T)(ptr - line);
	    hlf_T	spell_hlf = HLF_COUNT;

	    pos = wp->w_cursor;
	    wp->w_cursor.lnum = lnum;
	    wp->w_cursor.col = linecol;
	    len = spell_move_to(wp, FORWARD, TRUE, TRUE, &spell_hlf);

	    // spell_move_to() may call ml_get() and make "line" invalid
	    line = ml_get_buf(wp->w_buffer, lnum, FALSE);
	    ptr = line + linecol;

	    if (len == 0 || (int)wp->w_cursor.col > ptr - line)
	    {
		// no bad word found at line start, don't check until end of a
		// word
		spell_hlf = HLF_COUNT;
		word_end = (int)(spell_to_word_end(ptr, wp) - line + 1);
	    }
	    else
	    {
		// bad word found, use attributes until end of word
		word_end = wp->w_cursor.col + len + 1;

		// Turn index into actual attributes.
		if (spell_hlf != HLF_COUNT)
		    spell_attr = highlight_attr[spell_hlf];
	    }
	    wp->w_cursor = pos;

# ifdef FEAT_SYN_HL
	    // Need to restart syntax highlighting for this line.
	    if (has_syntax)
		syntax_start(wp, lnum);
# endif
	}
#endif
    }

    // Correct highlighting for cursor that can't be disabled.
    // Avoids having to check this for each character.
    if (fromcol >= 0)
    {
	if (noinvcur)
	{
	    if ((colnr_T)fromcol == wp->w_virtcol)
	    {
		// highlighting starts at cursor, let it start just after the
		// cursor
		fromcol_prev = fromcol;
		fromcol = -1;
	    }
	    else if ((colnr_T)fromcol < wp->w_virtcol)
		// restart highlighting after the cursor
		fromcol_prev = wp->w_virtcol;
	}
	if (fromcol >= tocol)
	    fromcol = -1;
    }

#ifdef FEAT_SEARCH_EXTRA
    if (!number_only)
    {
	v = (long)(ptr - line);
	area_highlighting |= prepare_search_hl_line(wp, lnum, (colnr_T)v,
					      &line, &screen_search_hl,
					      &search_attr);
	ptr = line + v; // "line" may have been updated
    }
#endif

#ifdef FEAT_SYN_HL
    // Cursor line highlighting for 'cursorline' in the current window.
    if (wp->w_p_cul && lnum == wp->w_cursor.lnum)
    {
	// Do not show the cursor line in the text when Visual mode is active,
	// because it's not clear what is selected then.  Do update
	// w_last_cursorline.
	if (!(wp == curwin && VIsual_active)
					 && wp->w_p_culopt_flags != CULOPT_NBR)
	{
	    cul_screenline = (wp->w_p_wrap
				   && (wp->w_p_culopt_flags & CULOPT_SCRLINE));

	    // Only set line_attr here when "screenline" is not present in
	    // 'cursorlineopt'.  Otherwise it's done later.
	    if (!cul_screenline)
	    {
		cul_attr = HL_ATTR(HLF_CUL);
		line_attr = cul_attr;
		wp->w_last_cursorline = wp->w_cursor.lnum;
	    }
	    else
	    {
		line_attr_save = line_attr;
		wp->w_last_cursorline = 0;
		margin_columns_win(wp, &left_curline_col, &right_curline_col);
	    }
	    area_highlighting = TRUE;
	}
	else
	    wp->w_last_cursorline = wp->w_cursor.lnum;
    }
#endif

#ifdef FEAT_PROP_POPUP
    {
	char_u *prop_start;

	text_prop_count = get_text_props(wp->w_buffer, lnum,
							   &prop_start, FALSE);
	if (text_prop_count > 0)
	{
	    // Make a copy of the properties, so that they are properly
	    // aligned.
	    text_props = ALLOC_MULT(textprop_T, text_prop_count);
	    if (text_props != NULL)
		mch_memmove(text_props, prop_start,
					 text_prop_count * sizeof(textprop_T));

	    // Allocate an array for the indexes.
	    text_prop_idxs = ALLOC_MULT(int, text_prop_count);
	    area_highlighting = TRUE;
	    extra_check = TRUE;
	}
    }
#endif

    off = (unsigned)(current_ScreenLine - ScreenLines);
    col = 0;

#ifdef FEAT_RIGHTLEFT
    if (wp->w_p_rl)
    {
	// Rightleft window: process the text in the normal direction, but put
	// it in current_ScreenLine[] from right to left.  Start at the
	// rightmost column of the window.
	col = wp->w_width - 1;
	off += col;
	screen_line_flags |= SLF_RIGHTLEFT;
    }
#endif

    // Repeat for the whole displayed line.
    for (;;)
    {
#if defined(FEAT_CONCEAL) || defined(FEAT_SEARCH_EXTRA)
	int has_match_conc  = 0;	// match wants to conceal
#endif
#ifdef FEAT_CONCEAL
	int did_decrement_ptr = FALSE;
#endif
	// Skip this quickly when working on the text.
	if (draw_state != WL_LINE)
	{
#ifdef FEAT_CMDWIN
	    if (draw_state == WL_CMDLINE - 1 && n_extra == 0)
	    {
		draw_state = WL_CMDLINE;
		if (cmdwin_type != 0 && wp == curwin)
		{
		    // Draw the cmdline character.
		    n_extra = 1;
		    c_extra = cmdwin_type;
		    c_final = NUL;
		    char_attr = hl_combine_attr(wcr_attr, HL_ATTR(HLF_AT));
		}
	    }
#endif