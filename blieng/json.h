
/* vim: set et ts=3 sw=3 ft=c:
 *
 * Copyright (C) 2012 James McLaughlin et al.  All rights reserved.
 * https://github.com/udp/json-parser
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _JSON_H
#define _JSON_H

#ifndef json_char
   #define json_char char
#endif

#ifndef json_int_t
   #ifndef _MSC_VER
      #include <inttypes.h>
#ifdef Q_OS_ANDROID
      #define json_int_t long
      #define json_uint_t unsigned long
#else
      #define json_int_t int64_t
      #define json_uint_t uint64_t
#endif
   #else
      #define json_int_t __int64
      #define json_uint_t __uint64
   #endif
#endif

#include <stdlib.h>

#ifdef __cplusplus

   #include <string.h>
   #include <string>

   extern "C"
   {

#endif

typedef struct
{
   unsigned long max_memory;
   int settings;

   /* Custom allocator support (leave null to use malloc/free)
    */

   void * (* mem_alloc) (size_t, int zero, void * user_data);
   void (* mem_free) (void *, void * user_data);

   void * user_data;  /* will be passed to mem_alloc and mem_free */

} json_settings;

#define json_relaxed_commas 1

typedef enum
{
   json_none,
   json_object,
   json_array,
   json_integer,
   json_uinteger,
   json_double,
   json_string,
   json_boolean,
   json_null

} json_type;

extern const struct _json_value json_value_none;

typedef struct _json_value
{
   struct _json_value * parent;

   json_type type;

   union
   {
      int boolean;
      json_int_t integer;
      double dbl;

      struct
      {
         unsigned int length;
         json_char * ptr; /* null terminated */

      } string;

      struct
      {
         unsigned int length;

         struct __values
         {
            json_char * name;
            struct _json_value * value;

         } * values;

         #if defined(__cplusplus) && __cplusplus >= 201103L
         decltype(values) begin () const
         {  return values;
         }
         decltype(values) end () const
         {  return values + length;
         }
         #elif defined(__cplusplus)
         class iterator{
         public:
            iterator(struct __values *ptr, unsigned int pos=0) : _ptr(ptr), _pos(pos) {}

            iterator operator++() {
                _pos++;
                return *this;
            }
            bool operator==(const iterator &cmp) {
                return (cmp._pos == _pos);
            }
            bool operator!=(const iterator &cmp) {
                return (cmp._pos != _pos);
            }
            struct __values operator*() {
                return _ptr[_pos];
            }

         private:
            struct __values *_ptr;
            unsigned int _pos;
         };
         iterator begin () const
         {
            iterator tmp(values);
            return tmp;
         }
         iterator end () const
         {
            iterator tmp(values, length);
            return tmp;
         }
         #endif

      } object;

      struct
      {
         unsigned int length;
         struct _json_value ** values;

         #if defined(__cplusplus) && __cplusplus >= 201103L
         decltype(values) begin () const
         {  return values;
         }
         decltype(values) end () const
         {  return values + length;
         }
         #elif defined(__cplusplus)
         class iterator{
         public:
            iterator(struct _json_value **ptr, unsigned int pos=0) : _ptr(ptr), _pos(pos) {}

            iterator operator++() {
                _pos++;
                return *this;
            }
            bool operator==(const iterator &cmp) {
                return (cmp._pos == _pos);
            }
            bool operator!=(const iterator &cmp) {
                return (cmp._pos != _pos);
            }
            struct _json_value *operator*() {
                return _ptr[_pos];
            }

         private:
            struct _json_value **_ptr;
            unsigned int _pos;
         };
         iterator begin () const
         {
            iterator tmp(values);
            return tmp;
         }
         iterator end () const
         {
            iterator tmp(values, length);
            return tmp;
         }
         #endif

      } array;

   } u;

   union
   {
      struct _json_value * next_alloc;
      void * object_mem;

   } _reserved;


   /* Some C++ operator sugar */

   #ifdef __cplusplus

      public:

         inline _json_value ()
         {  memset (this, 0, sizeof (_json_value));
         }

         inline const struct _json_value &operator [] (int index) const
         {
            if (type != json_array || index < 0
                     || ((unsigned int) index) >= u.array.length)
            {
               return json_value_none;
            }

            return *u.array.values [index];
         }

         inline const struct _json_value &operator [] (const char * index) const
         {
            if (type != json_object)
               return json_value_none;

            for (unsigned int i = 0; i < u.object.length; ++ i)
               if (!strcmp (u.object.values [i].name, index))
                  return *u.object.values [i].value;

            return json_value_none;
         }

         inline operator const char * () const
         {
            switch (type)
            {
               case json_string:
                  return u.string.ptr;

               default:
                  return "";
            };
         }

         inline const std::string asString () const
         {
            switch (type)
            {
               case json_string:
                  return u.string.ptr;

               default:
                  return "";
            };
         }

        inline operator json_int_t () const
        {
            return asInt();
        }

        inline json_int_t asInt () const
        {
            switch (type)
            {
               case json_uinteger:
               case json_integer:
                  return u.integer;

               case json_double:
                  return (json_int_t) u.dbl;

               default:
                  return 0;
            };
        }

        inline json_uint_t asUInt () const
        {
            switch (type)
            {
               case json_uinteger:
               case json_integer:
                  return (json_uint_t)u.integer;

               case json_double:
                  return (json_uint_t) u.dbl;

               default:
                  return 0;
            };
        }

        inline bool asBool() const
        {
            if (type != json_boolean)
               return false;

            return u.boolean != 0;
        }

        inline operator bool () const
        {
            return asBool();
        }

        inline double asDouble() const
        {
           switch (type)
           {
              case json_integer:
              case json_uinteger:
                 return (double) u.integer;

              case json_double:
                 return u.dbl;

              default:
                 return 0;
           };
        }

        inline operator double () const
        {
            return asDouble();
        }

        inline bool isArray () const
        {
            return (type == json_array);
        }

        inline bool isObject () const
        {
            return (type == json_object);
        }

        inline bool isString () const
        {
            return (type == json_string);
        }

        inline bool isNumeric () const
        {
            return (type == json_integer || type == json_uinteger || type == json_double);
        }

        inline bool isDouble () const
        {
            return (type == json_double);
        }

        inline bool isIntegral () const
        {
            return (type == json_integer || type == json_uinteger);
        }

        inline bool isMember (std::string key) const
        {
            if (type != json_object) return false;

            for (unsigned int i = 0; i < u.object.length; ++i) {
                if (u.object.values [i].name == key) {
                    return true;
                }
            }
            return false;
        }

        inline const struct _json_value *getMember (std::string key) const
        {
            if (type != json_object) return nullptr;

            for (unsigned int i = 0; i < u.object.length; ++i) {
                if (u.object.values [i].name == key) {
                    return u.object.values[i].value;
                }
            }

            return nullptr;
        }

        inline std::vector<std::string> getMemberNames() const
        {
            std::vector<std::string> res;
            if (type != json_object) return res;
            for (unsigned int i = 0; i < u.object.length; ++i) {
                res.push_back(u.object.values[i].name);
            }
            return res;
        }

   #endif

} json_value;

json_value * json_parse (const json_char * json,
                         size_t length);

#define json_error_max 128
json_value * json_parse_ex (json_settings * settings,
                            const json_char * json,
                            size_t length,
                            char * error);

void json_value_free (json_value *);


/* Not usually necessary, unless you used a custom mem_alloc and now want to
 * use a custom mem_free.
 */
void json_value_free_ex (json_settings * settings,
                         json_value *);


#ifdef __cplusplus
   } /* extern "C" */
#endif

#endif


