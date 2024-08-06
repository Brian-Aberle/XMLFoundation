// --------------------------------------------------------------------------
//			       Copyright United Business Technologies, Inc
// 				      (c) 1998 - 2016  All Rights Reserved.
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
//	Java Native Integration Interface for Object Oriented XML Serialization
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// --------------------------------------------------------------------------

// This article has some good ideas that will be applied to this code
// http://www.ibm.com/developerworks/library/j-jni/
//
#include "xmlObject.h"
#include "GString.h"
#include "GStringList.h"
#include "DynamicXMLObject.h"
#include "CacheManager.h"
#include "GException.h"
#include "GProfile.h"
#include <stdio.h>		// for : sscanf()
#include <stdlib.h>		// for : atof() 
#include <jni.h>



//JavaVM* gJavaVM = 0;
void ExchangeMembers(JNIEnv *env, const char *pzDir, DynamicXMLObject *pDO,	jobject jOb, DynamicXMLObject *pDOOwner);
void BuildMemberMaps(JNIEnv *env, DynamicXMLObject *pDO, jobject jOb);

#ifndef __JavaFoundation__cpp
#define __JavaFoundation__cpp

JavaVM* g_javaVM = 0;
jclass g_activityClassXXX = 0;
jobject g_activityObjXXX = 0;

GString g_JNIFoundationClass;
void setFoundationJNIClass(const char *pzClass)
{
	// "a777/root/GApp/GAppGlobal"
	g_JNIFoundationClass = pzClass;
}

// Android has no popen() implemented, so we will JNI up to the Java bootloader application
// which executes the same as popen() via code in the JVM.
void JavaShellExec(GString &strCommand, GString &strResult)
{
	JNIEnv *env;
	if (g_javaVM)	{
		g_javaVM->AttachCurrentThread(&env, NULL);
		// Get the instance of [public class GAppGlobal], then find the [String ShellExec(String)]
		jclass java_class = env->FindClass(g_JNIFoundationClass);
		if (java_class) {
			jmethodID shellexec = env->GetStaticMethodID(java_class, "ShellExec",	 "(Ljava/lang/String;)Ljava/lang/String;");
			if (shellexec) {
				// Construct a JNI String from the GString - then pass it to the java code in the 3rd arg to CallStaticObjectMethod()
				jstring jstr = env->NewStringUTF(strCommand.Buf());
				jstring strJava = (jstring) env->CallStaticObjectMethod(java_class, shellexec, jstr);
				// put the JNI String into the a GString strResult
				const char *strCpp = env->GetStringUTFChars(strJava, 0);
				strResult = strCpp;
				env->ReleaseStringUTFChars(strJava, strCpp);
			}
		}
	}
}

void logit(const char *pzLog){
	GString g(pzLog);
	g.ToFileAppend(GetProfile().GetStringOrDefault("System","LogFile",""),0);
}


union PortableCast
{
	jobject				mbrJobject;
	DynamicXMLObject *  mbrPtr;
	long				mbrInt; // jint
	void			 *  mbrVoid;	
};  

DynamicXMLObject *CastDXMLO(jint i)
{
	PortableCast pc;
	pc.mbrInt = i;
	DynamicXMLObject *pRet = cacheManager.isForeign(pc.mbrPtr);
	if (pRet)
		return pRet;  // a translated temporary handle
	return pc.mbrPtr; // i cast to a DynamicXMLObject *
}

jint CastDXMLO(DynamicXMLObject * p)
{
	PortableCast pc;
	pc.mbrPtr = p;
	return pc.mbrInt;
}


extern "C" JNIEXPORT jint JNICALL Java_a777_root_GApp_XMLObject_JavaConstruct  (JNIEnv *env, jobject jOb, jint n, jstring js, jint nAutoSync)
{
	if (!g_javaVM)
	{
		env->GetJavaVM(&g_javaVM);
	}
	
	jboolean isCopy;
    const char*pzXMLTag= env->GetStringUTFChars(js, &isCopy);
	DynamicXMLObject *pO = new DynamicXMLObject(pzXMLTag);
	pO->setAutoSync(nAutoSync);

	cacheManager.enterForeign( n,pO );	// enter the temporary index
	cacheManager.enterForeign( pO );	// enter the permanent index
	
	BuildMemberMaps(env, pO, jOb);// have the java object describe it's self
	
	// remove the temp handle
	cacheManager.tempToAlternate(n); 

	return CastDXMLO(pO);
}


void JavaDestructHelper(JNIEnv *env,DynamicXMLObject *pDXO)
{
	GListIterator it(pDXO->getSubUserLanguageObjects());
	while (it())
	{
		PortableCast pc;
		pc.mbrJobject = (jobject)it++;
		if (pc.mbrInt > 0)
		{
			// printf("$$$DelGlobRef[%d]\n",pc.mbrJobject);
			DynamicXMLObject *pDXOSub = cacheManager.DXOFromNative( pc.mbrInt );
			cacheManager.releaseForeign(pDXOSub);
			env->DeleteGlobalRef( pc.mbrJobject );
			delete pDXOSub;
		}
	}
}

extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaDestruct
  (JNIEnv *env, jobject jOb, jint iDO)
{
	DynamicXMLObject *pDXO = CastDXMLO(iDO);
	if (cacheManager.isForeign(pDXO))
	{
		GListIterator it(pDXO->getSubUserLanguageObjects());
		while (it())
		{
			PortableCast pc;
			pc.mbrJobject = (jobject)it++;
			if (pc.mbrInt > 0)
			{
				// printf("$$$DelGlobRef[%d]\n",pc.mbrJobject);
				DynamicXMLObject *pDXOSub = cacheManager.DXOFromNative( pc.mbrInt );
				cacheManager.releaseForeign(pDXOSub);
				env->DeleteGlobalRef( pc.mbrJobject );
				delete pDXOSub;
			}
		}
		cacheManager.releaseForeign(pDXO);
		delete pDXO;
	}
}


jobject MakeObjectInstance(JNIEnv *env,const char *pzObjectType,  DynamicXMLObject *pDO,DynamicXMLObject *pDXOOOwner)
{
	if (env->ExceptionOccurred()) 
	{
		env->ExceptionClear();
	}

	GString strClass(pzObjectType);
	int nStartAt = strClass.ReverseFind("/");
	if(nStartAt == -1)
		nStartAt = 0;
	else
		nStartAt++; // first byte past slash

	logit("MOI Make Object Instance1\n");
	logit(strClass);
	logit("\n");


//								"gapp/MyOrder");
	jclass clazzA = env->FindClass(strClass);

	// Here we are passing in a "variable" as a class name, these are the kids of errors that a bad value will produce:
	//
	//------------------------------------------------------ V ---------------
	//													  gapp/MyOrder    	works.
	//java.lang.ClassNotFoundException: Didn't find class "MyOrder" on
					  // JNI WARNING: illegal class name 'LMyOrder;' (FindClass)
					  // JNI WARNING: illegal class name 'Lgapp/MyOrder;' (FindClass)
								    //illegal class name 'MyOrder;' (FindClass)

	jobject objReturnValue = 0;
	GString strType("Ljava/lang/String;");
	// if this class type exposes a 'ctor that takes a single XMLObject we are using containment.
	jmethodID midctor = env->GetMethodID(clazzA, "<init>", "(LXMLObject;)V");
	if (env->ExceptionOccurred()) 
	{
		env->ExceptionClear();
	}
	if (midctor)
	{
		// create a new java XMLObject instance
		jclass clazzX = env->FindClass("XMLObject");
	    jmethodID midX = env->GetMethodID(clazzX, "<init>", "(Ljava/lang/String;)V");
		jstring	tagX = env->NewStringUTF(pDO->GetObjectTag());
		jobject objX = env->NewObject(clazzA, midX, tagX );

		// assign the object handle into the instance just created.
		jclass clazz = env->GetObjectClass(objX);
		jfieldID fid = env->GetFieldID(clazz, "oH", "I");
		env->SetIntField(objX, fid, CastDXMLO(pDO));

		// create a new instance of some user defined java class that is 
		// not extending XMLObject.  Pass the XMLObject to the 'ctor.
		objReturnValue = env->NewObject(clazzA, midctor, objX );
	}
	else
	{
		// create an instance of a java object derived from the java XMLObject
		objReturnValue = env->AllocObject(clazzA);

		// assign the base class object handle directly.
		jclass clazz = env->GetObjectClass(objReturnValue);

		// any object that extends XMLObject will have the oH (Object Handle)
		// If the Object created is a String the fid will be 0.
		jfieldID fid = env->GetFieldID(clazz, "oH", "I");
		if (env->ExceptionOccurred()) 
		{
			env->ExceptionClear();
		}
		if (fid)
		{
			env->SetIntField(objReturnValue, fid, CastDXMLO(pDO));

			// create the jobject to DXO index
			union CAST_THIS_TYPE_SAFE_COMPILERS
			{
				jobject   mbrObj;
				void *    mbrVoid;
			}Member;  

			Member.mbrObj = env->NewGlobalRef(objReturnValue);
			pDXOOOwner->addSubUserLanguageObject(Member.mbrVoid);
			// printf("===<Factory>NewGlobRef[%d]\n",Member.mbrObj);
			pDO->setUserLanguageObject(Member.mbrVoid);
			cacheManager.addAlternate( pDO );
		}
		else if (strType.CompareNoCase(pzObjectType) != 0)
		{
		  GString Err;
		  Err.Format("Object type [%s] must either be derived from XMLObject\n"
		  "or supply a constructor %s(XMLObject o)",pzObjectType,pzObjectType);
		}
	}
	pDO->SetObjectType(pzObjectType);
	return objReturnValue;
}

// get the (DynamicXMLObject *) from the given jobject
DynamicXMLObject *ExchangeDataObject(JNIEnv *env,  DynamicXMLObject *pUpdateFromDXO,  jobject job)
{
	// job is usually a Java object that extends XMLObject but it does not have to be.
	jclass clazz = env->GetObjectClass(job);
	jfieldID fid = env->GetFieldID(clazz, "oH", "I");
	DynamicXMLObject *pDOReturn = 0;
	if (fid) // then job does extend XMLObject
	{
		int iH = env->GetIntField(job, fid);
		pDOReturn = CastDXMLO(iH);
		pDOReturn->ClearPreviousMembers(pUpdateFromDXO);
	}
	else
	{
		// a Non-XMLObject.
		if (env->ExceptionOccurred()) 
		{
			env->ExceptionClear();
		}
	}
	
	return pDOReturn;
}


int UpdateExistingObject(JNIEnv *env, DynamicXMLObject *pJavaObjectDO,	 DynamicXMLObject * pToApplyDO, DynamicXMLObject *pDXOOwner)
{
    jboolean isCopy;
	jfieldID fid;

	GString strJavaOID;
	GString strNewOID;

	GStringIterator itKeyParts( pJavaObjectDO->GetKeyPartsList() );
	while(itKeyParts())
	{
		NativeMemberDescriptor *pMD = pJavaObjectDO->FindMemberDescriptor(itKeyParts++);

		PortableCast pc;
		pc.mbrJobject = (jobject)pJavaObjectDO->getUserLanguageObject();
		if (pc.mbrInt > 0)
		{
			jclass clazz = env->GetObjectClass(pc.mbrJobject);
			if (pMD->Source == 1) // Element
				strNewOID += pToApplyDO->GetMember(pMD->strTag);
			else if (pMD->Source == 2) // Attribute
				strNewOID += pToApplyDO->FindAttribute(pMD->strTag);


			switch(pMD->DataType)
			{
	/*long*/	case 0:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "J");
				strJavaOID += env->GetLongField(pc.mbrJobject, fid);
				break;
	/*double*/	case 1:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "D");
				strJavaOID << env->GetDoubleField(pc.mbrJobject, fid);
				break;

	/*short*/	case 2:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "S");
				strJavaOID << env->GetShortField(pc.mbrJobject, fid);
				break;
	/*byte*/	case 3:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "B");
				strJavaOID << env->GetByteField(pc.mbrJobject, fid);
				break;
	/*String*/	case 4:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "Ljava/lang/String;");
				strJavaOID += (char*) env->GetStringUTFChars( (jstring)env->GetObjectField(pc.mbrJobject,fid), &isCopy);
				break;

	/*int*/		case 5:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "I");
				strJavaOID << env->GetIntField(pc.mbrJobject, fid);
				break;

	/*boolean*/	case 6:
				fid = env->GetFieldID(clazz, (const char *)pMD->strName, "Z");
				strJavaOID << env->GetBooleanField(pc.mbrJobject, fid);
				break;
			}
			if (strJavaOID.CompareNoCase(strNewOID) == 0)
			{
				pJavaObjectDO->ClearPreviousMembers(pToApplyDO);
				ExchangeMembers(env,"in",pJavaObjectDO,pc.mbrJobject,pDXOOwner);
				return 1;	// Updated
			}
		}
		else
		{
			// attempted to retrieve an OID from a Root object.  Rare usage but
			// there could be some case where this needs to be done.  
		}
	}
	return 0;// Not Updated
}

int ApplyObjectToCollection(JNIEnv *env, jobject jobCollection, DynamicXMLObject *pDXOStateToApply,	DynamicXMLObject *pDXOOwner)
{
	jclass clazz2 = env->GetObjectClass(jobCollection);
	jmethodID midAddElement = env->GetMethodID(clazz2, "iterator", "()Ljava/util/Iterator;");
	jobject it = env->CallObjectMethod(jobCollection,midAddElement);

	clazz2 = env->GetObjectClass(it);
	jmethodID midNext = env->GetMethodID(clazz2,"next","()Ljava/lang/Object;");
	jmethodID midMore = env->GetMethodID(clazz2, "hasNext", "()Z");


	bool bUpdatedObject = false;
	jobject itjob;
	jfieldID fidOID;
	while( env->CallBooleanMethod(it,midMore) )
	{
		itjob = env->CallObjectMethod(it,midNext);
		clazz2 = env->GetObjectClass(itjob);
		fidOID = env->GetFieldID(clazz2, "oH", "I");
		DynamicXMLObject *pDXOJava =CastDXMLO(env->GetIntField(itjob, fidOID));
		
		// attempt to update this pDO(and it's java object) if the OID's match
		if (UpdateExistingObject(env, pDXOJava, pDXOStateToApply, pDXOOwner))
		{
			bUpdatedObject = true;
			return 1;								// Return "Updated"
		}
	}
	return 0;										// Return "Not updated"
}

DynamicXMLObject *nextSub(GListIterator &Iter,const char *pzTag)
{
	DynamicXMLObject *pDXOSubObject = 0;	
	while(Iter())
	{
		pDXOSubObject = (DynamicXMLObject *)Iter++;
		if (pDXOSubObject->IsXMLTag(pzTag) )
		{
			break;
		}
		pDXOSubObject = 0;	
	}
	return pDXOSubObject;
}

DynamicXMLObject *ApplyNesting(DynamicXMLObject *pDXOApply,  const char *pzNestedInTag)
{
	DynamicXMLObject *pTemp = 0;

	GStringList lst(" ",pzNestedInTag); // a space delimitted list of strings
	GStringIterator it(&lst);
	const char *pzTag = 0;
	while (it())
	{
		pzTag = it++;
		pTemp = pDXOApply->GetSubObject( pzTag );
		if (!pTemp)
			pTemp = pDXOApply->NewSubObject( pzTag );
		pDXOApply = pTemp;
	}
	return pDXOApply;
}


void DoExchange(JNIEnv *env, jobject jobParent, DynamicXMLObject *pDXOApply, const char *pzDirection, long nDataType,
				const char *pzVarName,const char *pzTag, const char *pzNestedInTag,	const char *pzObjectType,
				const char *pzContainerType,DynamicXMLObject *pDXOOwner, int Source)
{
	jmethodID midAddElement;
	jobject jobCollection;
    jclass clazz2;
	jobject job;
	jfieldID fid;

	DynamicXMLObject *pDXOSubObject = 0;
    jboolean isCopy;
	if (!jobParent || !env){
		logit("--------- Wasnt expecting this 1------------\n");
		return;

	}


	jclass clazz;
	try {
		clazz = env->GetObjectClass(jobParent);
	}catch(...) {
		logit("--------- Wasnt expecting this 1.5------------\n");
		return;
	}


	// for portable case insensitive compairs
	GString strStringType("Ljava/lang/String;");
	GString strOn("on");
	GString strTrue("true");
	GString strYes("yes");
	GString strOne("1");

	if (pzDirection[0] == 'i' || pzDirection[0] == 'I') // direction is 'in'
	{

		if (!pDXOApply){
			// this never happens
			return;
		}

		// Create a space delimitted list of strings
		GStringList lst(" ",pzNestedInTag); 
		GStringIterator it(&lst);
		while (it())
		{
			pDXOApply = pDXOApply->GetSubObject( it++ );
			if (!pDXOApply)
				return; // member [pzVarName] has no value in the XML
		}



//		logit("Tag=[");
//		logit(pzTag);						// TransactionTime
//		logit(" ");
//		logit(pDXOApply->GetObjectTag());   // Order
//		logit(" ");
//		logit(pDXOApply->GetVirtualType()); // Lgapp/MyOrder

		// The ObjectValue() of the first sub-Object matching the given xmlTag
		const char *pSrcValue = 0;
		if (Source == 1) // Element
			pSrcValue = pDXOApply->GetMember(pzTag);
		else if (Source == 2) // Attribute
			pSrcValue = pDXOApply->FindAttribute(pzTag);

//		logit("]Source=[");
//		logit(pSrcValue);
//		logit("]\n");


		jdouble d = 0;
		jlong	l = 0;
		jint i=0;
		jshort s = 0;
		jbyte b;
		jboolean z = 0;


		// Move data into the java object
		if (pSrcValue || nDataType > 6) // 7+ are objects
		{
			switch(nDataType)
			{
/*long*/	case 0:
			fid = env->GetFieldID(clazz, pzVarName, "J");
			l = Xatoi64(pSrcValue);
			env->SetLongField(jobParent, fid, l);
			break;
			
/*double*/	case 1:
			fid = env->GetFieldID(clazz, pzVarName, "D");
			d = atof(pSrcValue);
			env->SetDoubleField(jobParent, fid, d);
			break;
/*short*/	case 2:
			fid = env->GetFieldID(clazz, pzVarName, "S");
			sscanf (pSrcValue, "%hd",  &s); 
			env->SetShortField(jobParent, fid, s);
			break;
/*byte*/	case 3:
			fid = env->GetFieldID(clazz, pzVarName, "B");
			sscanf (pSrcValue, "%c",  &b); 
			env->SetByteField(jobParent, fid, b);
			break;
/*String*/	case 4:
			fid = env->GetFieldID(clazz, pzVarName, "Ljava/lang/String;");
			env->SetObjectField(jobParent, fid, env->NewStringUTF(pSrcValue));
			break;
/*int*/		case 5:
			fid = env->GetFieldID(clazz, pzVarName, "I");
			sscanf (pSrcValue, "%d",  &i); 
			env->SetIntField(jobParent, fid, i);
			break;
/*boolean*/	case 6:
			if (pSrcValue)
			{
				if (strOn.CompareNoCase(pSrcValue) == 0 ||
					strTrue.CompareNoCase(pSrcValue) == 0 ||
					strYes.CompareNoCase(pSrcValue) == 0  ||
					strOne.CompareNoCase(pSrcValue) == 0 )
				{
					z = 1;
				}
			}
			fid = env->GetFieldID(clazz, pzVarName, "Z");
			env->SetBooleanField(jobParent, fid, z);
			break;
/*Object*/	case 7:
			pDXOSubObject = pDXOApply->GetSubObject(pzTag);
			logit(pDXOSubObject->ToXML());
			logit("\n\n");
			if (pDXOSubObject)
			{
				// update it, otherwise create,assign,then update it
				GString strObjSignature;
				strObjSignature << "L" << pzObjectType << ";";
													 // "Lgapp/MyOrder;"
				fid = env->GetFieldID(clazz, pzVarName, strObjSignature);

				if (fid)
				{
					// handle to the java object reference
					job = env->GetObjectField(jobParent, fid);		//  OBTAIN
					if (!job)
					{
						//  CREATE then assign a new object into the java object reference
						job = MakeObjectInstance(env,pzObjectType, pDXOSubObject,pDXOOwner);
						env->SetObjectField(jobParent, fid, job);	//  ASSIGN
					}
					else
					{
					   pDXOSubObject=ExchangeDataObject(env,pDXOSubObject,job);
					}

					// move from the data object into the Java object //UPDATE
					ExchangeMembers(env, "in", pDXOSubObject, job, pDXOOwner);
				}
				else
				{
				// variable[pzVarName], type [pzObjectType] not found in object
					GString strError;
					strError << "ERROR @<" << pzTag << ">=" << pzVarName << " type:" << strObjSignature << " Not found\n";
					logit(strError);
					// otherwise we'll get java.lang.NoSuchFieldError: no field with name='objOrderXXX' signature='LMyOrder;' in class Lgapp/Customer1;
					env->ExceptionClear();
				}
			}
			break;

/*AbstractCollection of Objects*/
			case 8:

//			logit(pzTag);
//			logit("\n");
//			logit(pzContainerType);
//			logit(pzVarName);
//			logit("\n");


			// The source XML may specify 0..n object(s) of type 'pzTag'
			GListIterator Iter(pDXOApply->GetSubObjectList());
			pDXOSubObject = nextSub(Iter,pzTag);
			if (pDXOSubObject)
			{
				logit("adding object...\n");
				// get the container object				 Ljava/util/Vector;
				fid = env->GetFieldID(clazz, pzVarName,	 pzContainerType);
				if (!fid) {
  					// in this error the signature is wrong
					//	java.lang.NoSuchFieldError: no field with name='vecStrings' signature='java/util/Vector' in class Lgapp/Customer1;
					logit("**** FAILED Here!");
				}
			    if (fid)
				{
					logit("got fid\n");
					jobCollection = env->GetObjectField(jobParent, fid);
					logit("got collection\n");

					// create the collection object if necessary
					if (!jobCollection)
					{
						logit("NEW collection\n");
						logit(pzContainerType);
						logit("\n");

						GString strUnSignature(&pzContainerType[1], strlen(pzContainerType) - 2); // start at the 2nd byte and drop the last byte
						jclass ColClazz = env->FindClass(strUnSignature); // (pzContainerType);
						if (ColClazz) {
							logit("found collection\n");
							jmethodID midCtor = env->GetMethodID(ColClazz, "<init>", "()V");
							// CREATE
							logit("create object for collection\n");
							jobCollection = env->NewObject(ColClazz, midCtor);
							// ASSIGN
							logit("add object to collection\n");
							env->SetObjectField(jobParent, fid, jobCollection);
							logit("did it\n");
						}
						else{
							GString strError;
							strError << "ERROR @<" << pzTag << ">=" << pzVarName << " type:" << strUnSignature << " Not found\n";

							logit(strError);

						}
					}


					clazz2 = env->GetObjectClass(jobCollection);

					// add overload in Vector
					//@Override	public void add(int location, E object);
					//@Override	public synchronized boolean add(E object);
					midAddElement =	env->GetMethodID(clazz2,"add","(Ljava/lang/Object;)Z");
					if (midAddElement) {
						logit("OK midAddElement\n");

					}

					// env->GetMethodID(clazz2,"add","(I;Ljava/lang/Object;)V");
					// java.lang.NoSuchMethodError: no method with name='add' signature='(I;Ljava/lang/Object;)V' in class Ljava/util/Vector;


					// now add/update the object(s) in the collection.
					while (pDXOSubObject)
					{
						if (strStringType.CompareNoCase(pzObjectType) == 0) 
						{
							logit("Pre call Bool...\n");

							GString x;
							pDXOSubObject->ToXML(x);
							logit(x);
							logit("\n");


							// add a new String object into the collection;
							const char *pzVal =pDXOSubObject->GetObjectValue();

							logit("Post call Bool...\n");
							logit(pzVal);
							logit("\n");


							env->MonitorEnter(clazz2);
//							env->CallVoidMethod(jobCollection, midAddElement, env->NewStringUTF(pzVal));
							env->CallBooleanMethod(jobCollection, midAddElement, env->NewStringUTF(pzVal));


							logit(pzVal);
							logit(" Added\n");

							env->MonitorExit(clazz2);
	logit("added done\n");
						}
						else // it's a user defined object
						{	
							// Iterate the existing collection for an instance 
							// keyed by ObjectID.
	logit("add Object\n");
							if (!ApplyObjectToCollection(env,jobCollection,	pDXOSubObject, pDXOOwner))
							{
								logit("add Create\n");

								// No existing object with a matching ObjectID 
								// was found.  Create and Add a User defined 
								// object into the collection
								job = MakeObjectInstance(env,pzObjectType,pDXOSubObject,pDXOOwner);
								logit("add Create 2\n");
								// add object to collection

// this was the old NT/Solaris code ~2002
//								env->CallVoidMethod(jobCollection, midAddElement,job);

// changed for Android 2015
								env->CallBooleanMethod(jobCollection, midAddElement, job);


								ExchangeMembers(env, "in", pDXOSubObject,	job,pDXOOwner);
								logit("add Create 3\n");

							}
							else {
								logit("add Data Applied\n");

							}

						}

						// check the source XML for the next 
						// object to apply to this collection
						pDXOSubObject = nextSub(Iter,pzTag);
					}
				}
			}
			break;


			}// end of datatype switch
		}
		else
		{
			// xml tag[pzTag] declared in object[] but not found in XML.
			// ignore unless specifically told to do otherwise (v2)
		}
	}
	// direction is 'out'
	else if (pzDirection[0] == 'o' || pzDirection[0] == 'O') 
	{

		if (nDataType < 7)
			pDXOApply = ApplyNesting(pDXOApply,pzNestedInTag);
	
		GString strSrcValue;
		GString strSignature;

		// Move data out of the java object
		switch(nDataType) {
/*long*/    case 0:     // 64 bit where applicable
				fid = env->GetFieldID(clazz, pzVarName, "J");
				strSrcValue = env->GetLongField(jobParent, fid);
				break;

/*double*/    case 1:
				fid = env->GetFieldID(clazz, pzVarName, "D");
				strSrcValue = env->GetDoubleField(jobParent, fid);
				break;
/*short*/    case 2:
				fid = env->GetFieldID(clazz, pzVarName, "S");
				strSrcValue = env->GetShortField(jobParent, fid);
				break;
/*byte*/    case 3:
				fid = env->GetFieldID(clazz, pzVarName, "B");
				int i;
				i = (int) env->GetByteField(jobParent, fid);
				strSrcValue.Format("%d", i);
				break;
/*String*/    case 4:
				logit(pzVarName);
				logit("\n");

				fid = env->GetFieldID(clazz, pzVarName, "Ljava/lang/String;");
				job = env->GetObjectField(jobParent, fid);
				if (job) {
					strSrcValue = (char *) env->GetStringUTFChars((jstring)job, &isCopy);
				}
				else{
					logit("string object is NULL\n");
				}
			break;
/*int*/		case 5:
			fid = env->GetFieldID(clazz, pzVarName, "I");
			strSrcValue.Format("%d", env->GetIntField(jobParent, fid) );
			break;
/*boolean*/	case 6:
			fid = env->GetFieldID(clazz, pzVarName, "Z");
			if (env->GetBooleanField(jobParent, fid))
				strSrcValue	= "True";
			else
				strSrcValue	= "False";
			break;
/*object*/	case 7:
				logit("777 Where is our object?\n");

				logit(pzVarName);
				logit("\n");
				strSignature << "L" << pzObjectType << ";";
				logit(strSignature);
				logit("\n");

			    clazz = env->GetObjectClass(jobParent);

														//  "Lgapp/MyOrder;"  is what is must be
													    //   pzObjectType ==   gapp/MyOrder
				fid = env->GetFieldID(clazz, pzVarName,    strSignature );

//				fid = env->GetFieldID(g_activityClassXXX, pzVarName, pzObjectType); // we can use a global/static clazz

				if (fid) // if the referenced sub object, pzObjectType, is found in the parent
				{
					logit("!!!OBJECT!!!\n");
					job = env->GetObjectField(jobParent, fid);
					if (!job)	
					{
						// the reference is null, nothing to serialize.
						logit("the reference is null, nothing to serialize.\n");
					}
					else
					{
						logit("serialize object\n");

						// get the already created Storage for this jobject
						pDXOApply = ApplyNesting(pDXOApply,pzNestedInTag);
						logit("serialize object A\n");
						pDXOSubObject = ExchangeDataObject(env,pDXOSubObject,job);
						logit("serialize object B\n");
						pDXOApply->AddSubObject(pDXOSubObject);
						logit("serialize object C\n");
						ExchangeMembers(env, "out", pDXOSubObject, job, pDXOSubObject);
						logit("serialize object D\n");
					}
					
				}
				else
				{


logit("--Object NOT mapped ------------\n");
logit(pzVarName);
logit("\n");
logit(pzObjectType);



					// java.lang.NoSuchFieldError: no field with name='objOrderXXX' signature='gapp/MyOrder' in class Lgapp/Customer1;
					if (env->ExceptionOccurred())
					{
						env->ExceptionClear();
					}
				}
			break;
/*AbstractCollection of Objects*/
			case 8:
			
			// get the container object				 Ljava/util/Vector;
			fid = env->GetFieldID(clazz, pzVarName,	 pzContainerType);
			if (fid)
			{
				jobCollection = env->GetObjectField(jobParent, fid);// OBTAIN
				
				if (!jobCollection)
				{
					// Collection is null, (nothing to iterate or serialize)
				}
				else
				{
					// locate method used to add an object to the collection
					clazz2 = env->GetObjectClass(jobCollection);		
					
					// Iterate the collection
					midAddElement = env->GetMethodID(clazz2,"iterator", "()Ljava/util/Iterator;");
					jobject it = env->CallObjectMethod(jobCollection,midAddElement);

					clazz2 = env->GetObjectClass(it);
					jmethodID midNext =  env->GetMethodID(clazz2, "next", "()Ljava/lang/Object;");
					jmethodID midMore=env->GetMethodID(clazz2,"hasNext","()Z");
					
					jobject itjob;
					if( env->CallBooleanMethod(it,midMore) )
						pDXOApply = ApplyNesting(pDXOApply,pzNestedInTag);

					while( env->CallBooleanMethod(it,midMore) )
					{

						itjob = env->CallObjectMethod(it,midNext);

						if (strStringType.CompareNoCase(pzObjectType) == 0)
						{
							// it's a string object
							strSrcValue = (char*)
							   env->GetStringUTFChars((jstring)itjob, &isCopy);
							pDXOApply->SetMemberVar( pzTag, strSrcValue, 0 ); 
						}
						else
						{
							pDXOSubObject = ExchangeDataObject(env,0,itjob);
							pDXOOwner->addSubUserLanguageObject(pDXOSubObject->getUserLanguageObject());
							pDXOApply->AddSubObject(pDXOSubObject);
							ExchangeMembers(env,"out",pDXOSubObject,itjob, pDXOOwner);
						}
					}
				}
			}
			break;
		}
		
		// for all native types(int byte bool etc..) 
		if (nDataType < 7) 
		{
			if (Source == 1) // Element
				pDXOApply->SetMemberVar( pzTag, strSrcValue, 0); 
			else if (Source == 2) // Attribute
				pDXOApply->AddAttribute(pzTag, strSrcValue , 0); 
		}
	}
	else
	{
		// invalid direction
	}
}

void ExchangeMembers(JNIEnv *env, const char *pzDir, DynamicXMLObject *pDXO, jobject jOb,DynamicXMLObject *pDXOOwner)
{
	if (!pDXO)
		return;
	// for XMLObject derived classes, BuildMemberMaps() fills the members tree
	// for non-XMLObject derived classes, BuildMemberMaps() does nothing.
	BuildMemberMaps(env, pDXO, jOb);
	GBTreeIterator it(pDXO->GetMemberDescriptors());
	while(it())
	{
		try {

			NativeMemberDescriptor *pNMD = (NativeMemberDescriptor *)it++;
//			logit("NativeMemberDescriptor=[");
			const char *pzJavaVarName = pNMD->strName;
			const char *pzXMLTagInDXO = pNMD->strTag;
			int nDataType = pNMD->DataType;
//			logit(pzJavaVarName);
//			logit("] Tag <");
//			logit(pzXMLTagInDXO);
//			GString strLogit;
//			strLogit << "> [" << nDataType << "]\n";
//			logit(strLogit);
			DoExchange(env, jOb, pDXO, pzDir, nDataType, pzJavaVarName, pzXMLTagInDXO, pNMD->strWrapper,  pNMD->strObjectType,	pNMD->strContainerType, pDXOOwner, pNMD->Source);
		}
		catch(...) {
			// never happens
			logit("************this was unexpected***********\n");

		}
	}
	logit("-------------------EXIT\n");
}




extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaExchange
  (JNIEnv *env, jobject jObBase, jobject jOb2, jint iDO, jstring strDirection, 
   jint nDataType, jstring strVarName, jstring strTag, 
   jstring strNestedInTag, jstring strObjectType, jstring strContainerType,   jint nSource)
{
    jboolean isCopy;
    const char*pzDir= env->GetStringUTFChars(strDirection, &isCopy);
    const char*pzVarName= env->GetStringUTFChars(strVarName, &isCopy);
	const char*pzTag= env->GetStringUTFChars(strTag, &isCopy);
	const char*pzNestedTag= env->GetStringUTFChars(strNestedInTag, &isCopy);
	const char*pzObjectType= env->GetStringUTFChars(strObjectType, &isCopy);
	const char*pzContainerType= env->GetStringUTFChars(strContainerType, &isCopy);


	DynamicXMLObject *pDO = CastDXMLO(iDO);
	DoExchange(env, jOb2, pDO, pzDir, nDataType, pzVarName, pzTag, pzNestedTag,	pzObjectType, pzContainerType, pDO, nSource);
}

extern "C" JNIEXPORT jobject JNICALL Java_a777_root_GApp_XMLObject_getSubObj  (JNIEnv *env, jobject thisObj)
{
	// call the virtual test on the 'this'
    jclass clazz = env->GetObjectClass(thisObj);
    jmethodID mid = env->GetMethodID(clazz, "myVirtualTest", "()V");
    env->CallVoidMethod(thisObj, mid);

	jclass clazzA = env->FindClass("XMLObject");
	jobject objA = env->AllocObject(clazzA);

	jmethodID midctor = env->GetMethodID(clazzA, "<init>", "(LXMLObject;)V");
	if (midctor)
	{
		jobject objI = env->NewObject(clazzA, midctor, objA );
	}

	return objA;
}


extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaMapCacheDisable
  (JNIEnv *, jobject, jint iDO)
{
	DynamicXMLObject *pDO = CastDXMLO(iDO);
	pDO->DisableCache();
}

void BuildMemberMaps(JNIEnv *env, DynamicXMLObject *pDO, jobject jOb)
{
	GBTree *pMDTree = pDO->GetMemberDescriptors();

	// only build the member descriptor list once.  It is then cached.
	if (pDO->IsCacheDisabled() || pMDTree->isEmpty())
	{
		// empty the member Tree if necessary
		pDO->FreeMemberDescriptors();
		if (!pMDTree->isEmpty())
		{
			pMDTree->clear();
		}
		
		// call MapXMLTagsToMembers() allowing the virtual implementation to load pMDList through native Java calls to MapMember().
		jclass clazz = env->GetObjectClass(jOb);
		jmethodID mid = env->GetMethodID(clazz, "MapXMLTagsToMembers", "()V");
		if (mid)
		{
			env->CallVoidMethod(jOb, mid);
		}
		else
		{
			// object does not have a MapXMLTagsToMembers() implementation.
			// It is not required.  
			if (env->ExceptionOccurred()) 
			{
				env->ExceptionClear();
			}
		}
	}
}



extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaFromXML  (JNIEnv *env, jobject jOb, jint iDO, jstring strXML)
{
	logit("--inC++_JavaFromXML_1\n");


	env->GetJavaVM(&g_javaVM);
//	g_javaVM->AttachCurrentThread(&env, NULL);
	jclass clsXXX = env->GetObjectClass(jOb);
	g_activityClassXXX = (jclass) env->NewGlobalRef(clsXXX);
	g_activityObjXXX = env->NewGlobalRef(jOb);



	logit("--inC++_JavaFromXML\n");
	jboolean isCopy;
    const char*pzSourceXML= env->GetStringUTFChars(strXML, &isCopy);

	DynamicXMLObject *pDXO = CastDXMLO(iDO);


	PortableCast pc;
	pc.mbrVoid = pDXO->getUserLanguageObject();
	if (pc.mbrInt > 0)
	{
		// printf("$$$DelGlobRef[%d]\n",pc.mbrJobject);
		cacheManager.releaseForeign(pc.mbrInt);
		env->DeleteGlobalRef( pc.mbrJobject );
		pDXO->setUserLanguageObject((void *)-1);
	}

	try
	{
		pDXO->ClearPreviousMembers();
		BuildMemberMaps(env, pDXO, jOb);
		pDXO->FromXML(pzSourceXML);

//		logit(pzSourceXML);
//		logit("\n--------------------\n");
//		GString xx;
//		pDXO->ToXML(xx);
//		logit(xx._str);

		ExchangeMembers(env, "in", pDXO, jOb, pDXO);
	}
	catch( GException &rErr)
	{
	    jclass j = env->FindClass("java/lang/IllegalArgumentException");
		const char *pz = rErr.GetDescription();
		logit(pz);
		logit("exception\n");
		env->ThrowNew(j,pz);
	}
}



extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaMap  (JNIEnv *env, jobject jOb, jint iDO, jint DataType, jstring strName,	jstring strTag,
																jstring strWrapper, jstring strObjectType, jstring strContainerType, jint nSource)
{
	DynamicXMLObject *pDO = CastDXMLO(iDO);

	NativeMemberDescriptor *pJD = new NativeMemberDescriptor;
    jboolean isCopy;
	pJD->Source = nSource;
	pJD->DataType = DataType;
	pJD->strName = env->GetStringUTFChars(strName, &isCopy);
	pJD->strTag = env->GetStringUTFChars(strTag, &isCopy);
	pJD->strWrapper = env->GetStringUTFChars(strWrapper, &isCopy);
	pJD->strObjectType = env->GetStringUTFChars(strObjectType, &isCopy);
	pJD->strContainerType = env->GetStringUTFChars(strContainerType, &isCopy);
	pDO->AddMemberDescriptor(pJD);
}

extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaMapOID    (JNIEnv *env, jobject job, jint iDO, jobject jobThis, jstring k1, jstring k2, jstring k3, jstring k4, jstring k5)
{
	DynamicXMLObject *pDO = CastDXMLO(iDO);

	if (pDO->getUserLanguageObject() == 0)
	{
		void *pRef = (void *)env->NewGlobalRef(jobThis);
		// printf("===<Construct>NewGlobRef[%d]\n",pRef);
		pDO->setUserLanguageObject(pRef);
	}

    jboolean isCopy;
    const char*key1 = env->GetStringUTFChars(k1, &isCopy);
    const char*key2 = env->GetStringUTFChars(k2, &isCopy);
    const char*key3 = env->GetStringUTFChars(k3, &isCopy);
    const char*key4 = env->GetStringUTFChars(k4, &isCopy);
    const char*key5 = env->GetStringUTFChars(k5, &isCopy);
	GStringList *pGL = pDO->GetKeyPartsList();
	pGL->RemoveAll();
	pGL->AddLast(key1);
	if (key2 && key2[0])
		pGL->AddLast(key2);
	if (key3 && key3[0])
		pGL->AddLast(key3);
	if (key4 && key4[0])
		pGL->AddLast(key4);
	if (key5 && key5[0])
		pGL->AddLast(key5);
}


extern "C" JNIEXPORT jstring JNICALL Java_a777_root_GApp_XMLObject_JavaToXML  (JNIEnv *env, jobject jOb, jint iDO)
{
	env->GetJavaVM(&g_javaVM);
//	g_javaVM->AttachCurrentThread(&env, NULL);
	jclass clsXXX = env->GetObjectClass(jOb);
	g_activityClassXXX = (jclass) env->NewGlobalRef(clsXXX);
	g_activityObjXXX = env->NewGlobalRef(jOb);

	DynamicXMLObject *pDXO = CastDXMLO(iDO);
	if (pDXO->useAutoSync())
	{
		pDXO->ClearPreviousMembers();
		ExchangeMembers(env, "out", pDXO, jOb, pDXO);
	}
	jstring s = env->NewStringUTF(pDXO->getXML());
	pDXO->ClearPreviousMembers();
	return s;
}


extern "C" JNIEXPORT void JNICALL Java_a777_root_GApp_XMLObject_JavaRemoveAll
  (JNIEnv *, jobject, jint iDO)
{
	DynamicXMLObject *pDXO = CastDXMLO(iDO);
	pDXO->ClearPreviousMembers();
}
#endif // __JavaFoundation__cpp