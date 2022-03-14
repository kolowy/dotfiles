        if (netbeans_active() && (buf_signcount(wp->w_buffer, lnum) > 1))
        sprintf((char *)extra, "%-*c ", number_width(wp), SIGN_BYTE);
        text_sign = (sattr->sat_text != NULL) ? sattr->sat_typenr : 0;