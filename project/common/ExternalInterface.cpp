/*
Copyright (c) 2013, Hyperfiction
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef IPHONE
#define IMPLEMENT_API
#endif

#include <hx/CFFI.h>
#include <stdio.h>
#include <hxcpp.h>
//#include "HypPlayServices.h"
//using namespace hypplayservices;

#ifdef ANDROID
#include <jni.h>
#endif

#ifdef ANDROID
	extern JNIEnv *GetEnv();
	enum JNIType{
	   jniUnknown,
	   jniVoid,
	   jniObjectString,
	   jniObjectArray,
	   jniObject,
	   jniBoolean,
	   jniByte,
	   jniChar,
	   jniShort,
	   jniInt,
	   jniLong,
	   jniFloat,
	   jniDouble,
	};
	#define  LOG_TAG    "trace"
	#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#endif

AutoGCRoot *eval_onEvent = 0;
AutoGCRoot *eval_onEvent_multi = 0;
AutoGCRoot *eval_onDatas_multi = 0;

extern "C"{

	int HypPlayServices_register_prims(){
		printf("HypFacebook : register_prims()\n");
		return 0;
	}

	void HypPlayServices_onEvent( const char *sType , const char *sArg , int statusCode ){
		#ifdef ANDROID
		//ALOG("hypps_onEvent" );
		#endif
		int top = 0;
		gc_set_top_of_stack(&top,true);
		gc_exit_blocking();
		val_call3(
					eval_onEvent->get( ) ,
					alloc_string( sType ) ,
					alloc_string( sArg ),
					alloc_int( statusCode )
				);
		gc_enter_blocking();
	}

	void Multiplayers_onEvent( const char *sType , const char *sArg , int statusCode ){
		#ifdef ANDROID
		//ALOG("Multiplayers_onEvent" );
		#endif
		int top = 0;
		gc_set_top_of_stack(&top,true);
		gc_exit_blocking();
		val_call3(
					eval_onEvent_multi->get( ) ,
					alloc_string( sType ) ,
					alloc_string( sArg ),
					alloc_int( statusCode )
				);
		gc_enter_blocking();
	}

	void Multiplayers_onDatas( const char *sDatas , const char *sFrom ){
		#ifdef ANDROID
		//ALOG("Multiplayers_onDatas" );
		#endif
		val_call2(
					eval_onDatas_multi->get( ) ,
					alloc_string( sDatas ) ,
					alloc_string( sFrom )
				);
	}


// Common ------------------------------------------------------------------------------------------------------



// Android ----------------------------------------------------------------------------------------------------------

	#ifdef ANDROID

		JNIEXPORT void JNICALL Java_fr_hyperfiction_playservices_PlayServices_onEvent(
																	JNIEnv * env ,
																	jobject obj ,
																	jstring jsEvName,
																	jstring javaArg,
																	jint jStatusCode
																){
			const char *sEvName	= env->GetStringUTFChars( jsEvName , 0 );
			const char *sArg  	= env->GetStringUTFChars( javaArg , 0 );

			HypPlayServices_onEvent( sEvName , sArg , jStatusCode );

			env->ReleaseStringUTFChars( jsEvName	, sEvName );
			env->ReleaseStringUTFChars( javaArg 	, sArg );
		}

		JNIEXPORT void JNICALL Java_fr_hyperfiction_playservices_Multiplayers_onEvent(
																	JNIEnv * env ,
																	jobject obj ,
																	jstring jsEvName,
																	jstring javaArg,
																	jint jStatusCode
																){
			const char *sEvName	= env->GetStringUTFChars( jsEvName , 0 );
			const char *sArg  	= env->GetStringUTFChars( javaArg , 0 );

			Multiplayers_onEvent( sEvName , sArg , jStatusCode );

			env->ReleaseStringUTFChars( jsEvName	, sEvName );
			env->ReleaseStringUTFChars( javaArg 	, sArg );
		}

		JNIEXPORT void JNICALL Java_fr_hyperfiction_playservices_Multiplayers_onDatas(
																	JNIEnv * env ,
																	jobject obj ,
																	jstring jsDatas,
																	jstring jsFrom
																){
			const char *sDatas	= env->GetStringUTFChars( jsDatas , 0 );
			const char *sFrom  	= env->GetStringUTFChars( jsFrom , 0 );

			Multiplayers_onDatas( sDatas , sFrom );

			env->ReleaseStringUTFChars( jsDatas	, sDatas );
			env->ReleaseStringUTFChars( jsFrom 	, sFrom );
		}

		JNIEXPORT void JNICALL Java_fr_hyperfiction_playservices_HypPlayServicesFrag_onEvent(
																	JNIEnv * env ,
																	jobject obj ,
																	jstring jsEvName,
																	jstring javaArg
																){
			const char *sEvName	= env->GetStringUTFChars( jsEvName , 0 );
			const char *sArg  	= env->GetStringUTFChars( javaArg , 0 );

			HypPlayServices_onEvent( sEvName , sArg , 0 );

			env->ReleaseStringUTFChars( jsEvName	, sEvName );
			env->ReleaseStringUTFChars( javaArg 	, sArg );
		}

	#endif
}

// Callbacks ------------------------------------------------------------------------------------------------------

	static value HypPlayServices_set_event_callback( value onCall ){
		eval_onEvent = new AutoGCRoot( onCall );
		return alloc_bool( true );
	}
	DEFINE_PRIM( HypPlayServices_set_event_callback , 1 );

	static value HypPlayServices_set_event_callback_multiplayers( value onCall ){
		eval_onEvent_multi = new AutoGCRoot( onCall );
		return alloc_bool( true );
	}
	DEFINE_PRIM( HypPlayServices_set_event_callback_multiplayers , 1 );

	static value HypPlayServices_set_datas_callback_multiplayers( value onCall ){
		eval_onDatas_multi = new AutoGCRoot( onCall );
		return alloc_bool( true );
	}
	DEFINE_PRIM( HypPlayServices_set_datas_callback_multiplayers , 1 );

// iPhone ---------------------------------------------------------------------------------------------------------

#ifdef IPHONE

#endif

