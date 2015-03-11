///////////////////////////////////////////////////////////////////////////////
///  @file class_public_member.h
///  Getter and setter functions for the internal classes public member
///  variables.
///  @remarks
///  A getter for a public member(M) of a class(C) creates a function C::get_M
///  A setter similary creates a function C::set_M similary
///  @author Liam Devine
///  @email
///  See http://www.liamdevine.co.uk for contact details.
///  @licence
///  @licence 
///  See licence.txt for more details. \n 
///////////////////////////////////////////////////////////////////////////////

#ifndef CLASS_PUBLIC_MEMBER_H_
#	define CLASS_PUBLIC_MEMBER_H_

#include "fwd_push_pull.h"
#include "param_traits.h"

namespace OOLUA 
{
	namespace INTERNAL
	{
	
		template<typename T,int shouldBeByRef>
		struct shouldPushValueByReference 
		{
			enum {value = 0};
			static void push(lua_State* l,T* input)
			{
				OOLUA::push2lua(l,*input);		
			}
		};
	
		template<typename T>
		struct shouldPushValueByReference<T,1>
		{
			enum {value = 1};
			static void push(lua_State* l ,T* input)
			{
				OOLUA::push2lua(l,input);
			}
		};
	
		struct PushPublicMember 
		{
			template<typename T>
			static void push(lua_State* l, T* input)
			{
				shouldPushValueByReference<T, 
					//can not have a pointer to reference.
					//T will either be a pointer to type
					//or type
					OOLUA::INTERNAL::is_param_by_value<T>::value 
					&& has_a_proxy_type<typename OOLUA::INTERNAL::Raw_type<T>::type >::value >
						::push(l,input);
			}
		
		};
		
	}
}

#define OOLUA_PUBLIC_MEMBER_SET(id)\
int set_##id(lua_State* l)\
{\
	OOLUA::INTERNAL::LUA_CALLED::pull2cpp(l,m_this->id);\
 	return 0;\
}


#define OOLUA_PUBLIC_MEMBER_GET(id)\
int get_##id(lua_State* l) const\
{\
	OOLUA::INTERNAL::PushPublicMember::push(l,&m_this->id);\
	return 1;\
}


#define OOLUA_PUBLIC_MEMBER_GET_SET(id)\
OOLUA_PUBLIC_MEMBER_GET(id)\
OOLUA_PUBLIC_MEMBER_SET(id)



#endif //CLASS_PUBLIC_MEMBER_H_
