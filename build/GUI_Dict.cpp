// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME builddIGUI_Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "include/GUImain.hpp"
#include "include/analysis.hpp"
#include "include/block.hpp"
#include "include/alias.hpp"
#include "include/mem_root.hpp"
#include "include/output.hpp"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *MyClass_Dictionary();
   static void MyClass_TClassManip(TClass*);
   static void *new_MyClass(void *p = nullptr);
   static void *newArray_MyClass(Long_t size, void *p);
   static void delete_MyClass(void *p);
   static void deleteArray_MyClass(void *p);
   static void destruct_MyClass(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MyClass*)
   {
      ::MyClass *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MyClass));
      static ::ROOT::TGenericClassInfo 
         instance("MyClass", "include/MyClass.hpp", 18,
                  typeid(::MyClass), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MyClass_Dictionary, isa_proxy, 4,
                  sizeof(::MyClass) );
      instance.SetNew(&new_MyClass);
      instance.SetNewArray(&newArray_MyClass);
      instance.SetDelete(&delete_MyClass);
      instance.SetDeleteArray(&deleteArray_MyClass);
      instance.SetDestructor(&destruct_MyClass);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MyClass*)
   {
      return GenerateInitInstanceLocal(static_cast<::MyClass*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MyClass*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MyClass_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::MyClass*>(nullptr))->GetClass();
      MyClass_TClassManip(theClass);
   return theClass;
   }

   static void MyClass_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_outputFrame(void *p);
   static void deleteArray_outputFrame(void *p);
   static void destruct_outputFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::outputFrame*)
   {
      ::outputFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::outputFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("outputFrame", ::outputFrame::Class_Version(), "include/output.hpp", 18,
                  typeid(::outputFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::outputFrame::Dictionary, isa_proxy, 4,
                  sizeof(::outputFrame) );
      instance.SetDelete(&delete_outputFrame);
      instance.SetDeleteArray(&deleteArray_outputFrame);
      instance.SetDestructor(&destruct_outputFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::outputFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::outputFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::outputFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_MyMainFrame(void *p);
   static void deleteArray_MyMainFrame(void *p);
   static void destruct_MyMainFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MyMainFrame*)
   {
      ::MyMainFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MyMainFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MyMainFrame", ::MyMainFrame::Class_Version(), "include/GUImain.hpp", 20,
                  typeid(::MyMainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MyMainFrame::Dictionary, isa_proxy, 4,
                  sizeof(::MyMainFrame) );
      instance.SetDelete(&delete_MyMainFrame);
      instance.SetDeleteArray(&deleteArray_MyMainFrame);
      instance.SetDestructor(&destruct_MyMainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MyMainFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::MyMainFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MyMainFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr outputFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *outputFrame::Class_Name()
{
   return "outputFrame";
}

//______________________________________________________________________________
const char *outputFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::outputFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int outputFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::outputFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *outputFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::outputFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *outputFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::outputFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MyMainFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MyMainFrame::Class_Name()
{
   return "MyMainFrame";
}

//______________________________________________________________________________
const char *MyMainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MyMainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MyMainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MyMainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MyClass(void *p) {
      return  p ? new(p) ::MyClass : new ::MyClass;
   }
   static void *newArray_MyClass(Long_t nElements, void *p) {
      return p ? new(p) ::MyClass[nElements] : new ::MyClass[nElements];
   }
   // Wrapper around operator delete
   static void delete_MyClass(void *p) {
      delete (static_cast<::MyClass*>(p));
   }
   static void deleteArray_MyClass(void *p) {
      delete [] (static_cast<::MyClass*>(p));
   }
   static void destruct_MyClass(void *p) {
      typedef ::MyClass current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::MyClass

//______________________________________________________________________________
void outputFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class outputFrame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(outputFrame::Class(),this);
   } else {
      R__b.WriteClassBuffer(outputFrame::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_outputFrame(void *p) {
      delete (static_cast<::outputFrame*>(p));
   }
   static void deleteArray_outputFrame(void *p) {
      delete [] (static_cast<::outputFrame*>(p));
   }
   static void destruct_outputFrame(void *p) {
      typedef ::outputFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::outputFrame

//______________________________________________________________________________
void MyMainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MyMainFrame.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MyMainFrame::Class(),this);
   } else {
      R__b.WriteClassBuffer(MyMainFrame::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MyMainFrame(void *p) {
      delete (static_cast<::MyMainFrame*>(p));
   }
   static void deleteArray_MyMainFrame(void *p) {
      delete [] (static_cast<::MyMainFrame*>(p));
   }
   static void destruct_MyMainFrame(void *p) {
      typedef ::MyMainFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::MyMainFrame

namespace {
  void TriggerDictionaryInitialization_GUI_Dict_Impl() {
    static const char* headers[] = {
"include/GUImain.hpp",
"include/analysis.hpp",
"include/block.hpp",
"include/alias.hpp",
"include/mem_root.hpp",
"include/output.hpp",
nullptr
    };
    static const char* includePaths[] = {
"/usr/local/root/include/",
"/home/otokun241/newRepository/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "GUI_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$include/GUImain.hpp")))  MyClass;
class __attribute__((annotate("$clingAutoload$include/GUImain.hpp")))  outputFrame;
class __attribute__((annotate("$clingAutoload$include/GUImain.hpp")))  MyMainFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "GUI_Dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/GUImain.hpp"
#include "include/analysis.hpp"
#include "include/block.hpp"
#include "include/alias.hpp"
#include "include/mem_root.hpp"
#include "include/output.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"MyClass", payloadCode, "@",
"MyMainFrame", payloadCode, "@",
"outputFrame", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("GUI_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_GUI_Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_GUI_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_GUI_Dict() {
  TriggerDictionaryInitialization_GUI_Dict_Impl();
}
