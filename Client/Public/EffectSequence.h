//#pragma once
//
//#include "ImSequencer.h"
//#include "Base.h"
//
//NS_BEGIN(Client)
//
//class CEffectSequence : public ImSequencer::SequenceInterface
//{
//private:
//    CEffectSequence() = default;
//    ~CEffectSequence() = default;
//
//public:
//    virtual int GetFrameMin() const override {};
//    virtual int GetFrameMax() const override {};
//    virtual int GetItemCount() const override {};
//
//    virtual void BeginEdit(int /*index*/)  override {}
//    virtual void EndEdit() override {}
//    virtual int GetItemTypeCount() const  override { return 0; }
//    virtual const char* GetItemTypeName(int /*typeIndex*/) const override { return ""; }
//    virtual const char* GetItemLabel(int /*index*/) const override { return ""; }
//    virtual const char* GetCollapseFmt() const override { return "%d Frames / %d entries"; }
//
//    virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) override {};
//    virtual void Add(int /*type*/)  override {}
//    virtual void Del(int /*index*/) override {}
//    virtual void Duplicate(int /*index*/) override {}
//
//    virtual void Copy() override {}
//    virtual void Paste()  override {}
//
//    virtual size_t GetCustomHeight(int /*index*/) override { return 0; }
//    virtual void DoubleClick(int /*index*/) override {}
//
//private:
//    vector<SpriteEffects*> SpriteEffects;
//};
//
//NS_END