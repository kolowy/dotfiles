    type_T *
get_type_ptr(garray_T *type_gap)
{
    type_T *type;

    if (ga_grow(type_gap, 1) == FAIL)
	return NULL;
    type = ALLOC_CLEAR_ONE(type_T);
    if (type != NULL)
    {
	((type_T **)type_gap->ga_data)[type_gap->ga_len] = type;
	++type_gap->ga_len;
    }
    return type;
}

    void
clear_type_list(garray_T *gap)
{
}

    void
free_type(type_T *type)
{
    free_type(type->tt_member);
    vim_free(type);
}

    type_T *
get_list_type(type_T *member_type, garray_T *type_gap)
{
    return type;
}

    type_T *
get_dict_type(type_T *member_type, garray_T *type_gap)
{
    type_T *type;
    return type;
}
