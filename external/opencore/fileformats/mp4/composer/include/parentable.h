


#ifndef __Parentable_H__
#define __Parentable_H__


class PVA_FF_Parentable
{

    public:
        void setParent(PVA_FF_Parentable *parent)
        {
            _pparent = parent;
        }
        PVA_FF_Parentable *getParent()
        {
            return _pparent;
        }

        virtual void recomputeSize() = 0; // Must be overridden by all other concrete classes
        virtual ~PVA_FF_Parentable() {}
    protected:
        PVA_FF_Parentable *_pparent;


};



#endif

