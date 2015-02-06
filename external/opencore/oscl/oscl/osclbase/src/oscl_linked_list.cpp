
#include "oscl_linked_list.h"



OSCL_EXPORT_REF void Oscl_Linked_List_Base::construct(Oscl_Opaque_Type_Alloc_LL* op)
{
    num_elements = 0;
    head = tail = iterator = NULL;
    pOpaqueType = op;
}

OSCL_EXPORT_REF void Oscl_Linked_List_Base::destroy()
{
    OsclAny* tmp;
    while (num_elements && head)
    {
        tmp = pOpaqueType->get_next(head);
        pOpaqueType->destroy(head);
        pOpaqueType->deallocate(head);
        --num_elements;
        head = tmp;
    }
    head = tail = iterator = NULL;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::get_first(OsclAny* ele)
{
    if (!head) return 0;
    iterator = head;
    pOpaqueType->get_data(iterator, ele);
    return 1;
};

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::get_next(OsclAny* ele)
{
    if (iterator == tail) return 0;
    if (! iterator)
    {
        if (!head) return 0;
        iterator = head;
    }
    else
    {
        iterator = pOpaqueType->get_next(iterator);
    }
    pOpaqueType->get_data(iterator, ele);
    return 1;
};

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::check_list()
{
    OsclAny *tmp;

    int32 ii;
    for (tmp = head, ii = 0; tmp ; ++ii)
        tmp = pOpaqueType->get_next(tmp);

    return (ii == num_elements);
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::add_element(OsclAny* new_element)
{
    if (!tail)
    {
        OsclAny* ptr = pOpaqueType->allocate(sizeof_T);
        pOpaqueType->construct(ptr, new_element);
        head = tail = ptr;
        if (head == NULL)
            return 0; // Memory Allocation Error
    }
    else
    {
        OsclAny* ptr = pOpaqueType->allocate(sizeof_T);
        pOpaqueType->construct(ptr, new_element);
        pOpaqueType->set_next(tail, ptr);
        if (pOpaqueType->get_next(tail) == NULL)
            return 0; // Memory Allocation Error
        tail = pOpaqueType->get_next(tail);
    }

    ++num_elements;
    return 1;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::add_to_front(const OsclAny* new_element)
{
    if (!head)
    {
        OsclAny* ptr = pOpaqueType->allocate(sizeof_T);
        pOpaqueType->construct(ptr, new_element);
        head = tail = ptr;
        if (head == NULL)
            return 0; // Memory Allocation Error
    }
    else
    {
        OsclAny* ptr = pOpaqueType->allocate(sizeof_T);
        pOpaqueType->construct(ptr, new_element);
        if (ptr == NULL)
            return 0; // Memory Allocation Error
        pOpaqueType->set_next(ptr, head);
        head = ptr;
    }
    ++num_elements;
    return 1;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::get_element(int32 index, OsclAny* element)
{

    OsclAny *tmp;
    int32 ii;

    if (index < 0 || index >= num_elements)
    {
        return 0;
    }

    for (tmp = head, ii = 0; ii < index; ++ii, tmp = pOpaqueType->get_next(tmp))
    {
    }

    pOpaqueType->get_data(tmp, element);
    return 1;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::remove_element(const OsclAny* data_to_remove)
{
    OsclAny *tmp;
    OsclAny *prev;
    int32 found = 0;

    for (tmp = head, prev = NULL; tmp; prev = tmp, tmp = pOpaqueType->get_next(tmp))
    {

        if (pOpaqueType->compare_data(tmp, data_to_remove))
        {
            found = 1;
            if (prev)
            {
                pOpaqueType->set_next(prev, pOpaqueType->get_next(tmp));
                if (iterator == tmp) iterator = prev;
            }
            else
            {
                head = pOpaqueType->get_next(tmp);
                if (iterator == tmp) iterator = NULL;
            }
            if (tmp == tail)
            {
                tail = prev;
            }
            pOpaqueType->destroy(tmp);
            pOpaqueType->deallocate(tmp);
            --num_elements;
            break;
        }

    }
    return found;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::get_index(const OsclAny* data)
{
    OsclAny *tmp;
    int32 index = 0;
    int32 found = 0;

    for (tmp = head, index = 0; tmp; tmp = pOpaqueType->get_next(tmp), ++index)
    {

        if (pOpaqueType->compare_data(tmp, data))
        {
            found = 1;
            break;
        }
    }
    if (found)
        return index;

    return -1;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::remove_element(const int32 index_to_remove)
{
    OsclAny *tmp;
    OsclAny *prev;
    int32 ii;

    if (index_to_remove < 0 || index_to_remove >= num_elements)
    {
        return 0;
    }

    // skip to desired element
    for (tmp = head, prev = NULL, ii = 0; tmp && ii < index_to_remove;
            ++ii, prev = tmp, tmp = pOpaqueType->get_next(tmp))
    {
    }

    if (ii != index_to_remove)
    {
        return 0;
    }

    if (prev)
    {
        pOpaqueType->set_next(prev, pOpaqueType->get_next(tmp));
        if (iterator == tmp) iterator = prev;
    }
    else
    {
        head = pOpaqueType->get_next(tmp);
        if (iterator == tmp) iterator = NULL;
    }
    if (tmp == tail)
    {
        tail = prev;
    }

    pOpaqueType->destroy(tmp);
    pOpaqueType->deallocate(tmp);
    --num_elements;

    return 1;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::move_to_end(const OsclAny* data_to_move)
{
    OsclAny *tmp;
    OsclAny *prev;
    uint32 found = 0;

    for (tmp = head, prev = NULL; tmp; prev = tmp, tmp = pOpaqueType->get_next(tmp))
    {

        if (pOpaqueType->compare_data(tmp, data_to_move))
        {
            found = 1;
            if (tmp == tail)
            {
                return 1; // nothing to do
            }
            if (prev)
            {
                pOpaqueType->set_next(prev, pOpaqueType->get_next(tmp));
                if (iterator == tmp) iterator = prev;
            }
            if (tmp == head)
            {
                head = pOpaqueType->get_next(tmp);
                if (iterator == tmp) iterator = NULL;
            }
            pOpaqueType->set_next(tail, tmp);
            pOpaqueType->set_next(tmp, NULL);
            tail = tmp;

            return 1;
        }
    }

    return 0;
}

OSCL_EXPORT_REF int32 Oscl_Linked_List_Base::move_to_front(const OsclAny* data_to_move)
{
    OsclAny *tmp;
    OsclAny *prev;
    int32 found = 0;

    for (tmp = head, prev = NULL; tmp; prev = tmp, tmp = pOpaqueType->get_next(tmp))
    {

        if (pOpaqueType->compare_data(tmp, data_to_move))
        {
            found = 1;
            if (tmp == head)
            {
                return 1; // nothing to do
            }

            if (prev)
            {
                pOpaqueType->set_next(prev, pOpaqueType->get_next(tmp));

                if (iterator == tmp)
                {
                    iterator = prev;
                }
            }

            if (tmp == tail)
            {
                tail = prev;
            }
            pOpaqueType->set_next(tmp, head);
            head = tmp;

            return 1;
        }
    }

    return 0;
}



