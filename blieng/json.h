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
      #define json_int_t long
      #define json_uint_t unsigned long
   #else
      #define json_int_t long
      #define json_uint_t unsigned long
   #endif
#endif

#include <stdlib.h>

#ifdef __cplusplus

   #include <string.h>
   #include <string>

   extern "C"
   {

#endif

/**
 * JSON settings for parsing.
 */
typedef struct
{
   unsigned long max_memory; //!< Maximum memory to use
   int settings; //!< Settings flags

   /**
    * Custom allocator support (leave null to use malloc/free)
    *
    * \param alloc_size Number of bytes to alloc
    * \param zero Zero allocated memory? 0 not to, any other to zero
    * \param user_data User defined data for alloc method
    * \returns Pointer to beginning of allocated memory
    */
   void * (* mem_alloc) (size_t alloc_size, int zero, void * user_data);
   /**
    * Custom allocator support (leave null to use malloc/free)
    *
    * \param ptr Pointer to beginnig of allocated memory
    * \param user_data User defined data for alloc method
    * \returns Pointer to beginning of allocated memory
    */
   void (* mem_free) (void *ptr, void * user_data);

   void * user_data; //!< Will be passed to mem_alloc and mem_free

} json_settings;

#define json_relaxed_commas 1

/**
 * Different JSON object types.
 */
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

/**
 * Structure to contain parsed JSON value.
 */
typedef struct _json_value
{
   struct _json_value * parent; //!< Parent of this value or nullptr

   json_type type; //!< Type of this value

   union
   {
      int boolean; //!< Data in boolean type
      json_int_t integer; //!< Data in integer type
      double dbl; //!< Data in double type

      /**
       * Data if type is string
       */
      struct
      {
         unsigned int length; //!< Length of the string
         json_char * ptr; //!< Null terminated string
      } string;

      /**
       * Data for object type
       */
      struct
      {
         unsigned int length; //!< Length of the object, ie. number of key-value pairs

         struct __values
         {
            json_char * name; //!< Name of the value
            struct _json_value * value; //!< The value itself
         } * values; //!<List of values as key-value pairs

         #if defined(__cplusplus) && __cplusplus >= 201103L
         /**
          * Start iterator for values
          *
          * \returns Begin iterator
          */
         decltype(values) begin () const {
            return values;
         }
         /**
          * End iterator for values
          *
          * \returns End iterator
          */
         decltype(values) end () const {
            return values + length;
         }
         #elif defined(__cplusplus)
         /**
          * Iterator for older C++ implementations, ie. pre C++03
          */
         class iterator {
         public:
            iterator(struct __values *ptr, unsigned int pos=0) : _ptr(ptr), _pos(pos) {}

            iterator operator++() {
                _pos++;
                return *this;
            }
            bool operator==(const iterator &cmp) const {
                return (cmp._pos == _pos);
            }
            bool operator!=(const iterator &cmp) const {
                return (cmp._pos != _pos);
            }
            struct __values operator*() const {
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

      /**
       * Data for arrays
       */
      struct
      {
         unsigned int length; //!< Length of the array
         struct _json_value ** values; //!< Array of value objects

         #if defined(__cplusplus) && __cplusplus >= 201103L
         /**
          * Start iterator for array values
          *
          * \returns Begin iterator
          */
         decltype(values) begin () const {
            return values;
         }
         /**
          * End iterator for array values
          *
          * \returns End iterator
          */
         decltype(values) end () const {
            return values + length;
         }
         #elif defined(__cplusplus)
         /**
          * Iterator for older C++ implementations, ie. pre C++03
          */
         class iterator {
         public:
            iterator(struct _json_value **ptr, unsigned int pos=0) : _ptr(ptr), _pos(pos) {}

            iterator operator++() {
                _pos++;
                return *this;
            }
            bool operator==(const iterator &cmp) const {
                return (cmp._pos == _pos);
            }
            bool operator!=(const iterator &cmp) const {
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

   /**
    * Linked list for next value allocation
    */
   union
   {
      struct _json_value * next_alloc; //!< Next allocated value
      void * object_mem; //!< Reservation for object memory
   } _reserved;


   /* Some C++ operator sugar */

   #ifdef __cplusplus

      public:
         /**
          * Initialize empty value
          */
         inline _json_value () {
            memset (this, 0, sizeof (_json_value));
         }

         /**
          * Easy array access for item in array.
          *
          * \param index Numbered index of item
          * \returns The value matching the number
          */
         inline const struct _json_value &operator [] (int index) const
         {
            if (type != json_array || index < 0
                     || ((unsigned int) index) >= u.array.length)
            {
               return json_value_none;
            }

            return *u.array.values [index];
         }

         /**
          * Easy array access for item in object.
          *
          * \param index Name of object to get
          * \returns The value matching the name
          */
         inline const struct _json_value &operator [] (const char * index) const
         {
            if (type != json_object)
               return json_value_none;

            for (unsigned int i = 0; i < u.object.length; ++ i)
               if (!strcmp (u.object.values [i].name, index))
                  return *u.object.values [i].value;

            return json_value_none;
         }

         /**
          * Access for string value.
          * If type is string, return the string as char array
          *
          * \returns String as char array or empty string
          */
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

         /**
          * Access for string value.
          * If type is string, return the string as std::string
          *
          * \returns String as std::string or empty string
          */
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

        /**
         * Get object value as integer.
         *
         * \returns Value as integer
         */
        inline operator json_int_t () const
        {
            return asInt();
        }

        /**
         * Get object value as integer.
         *
         * \returns Value as integer
         */
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

        /**
         * Get object value as unsigned integer.
         *
         * \returns Value as unsigned integer
         */
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

        /**
         * Get object value as boolean.
         *
         * \returns Value as boolean
         */
        inline bool asBool() const
        {
            if (type != json_boolean)
               return false;

            return u.boolean != 0;
        }

        /**
         * Get object value as boolean.
         *
         * \returns Value as boolean
         */
        inline operator bool () const
        {
            return asBool();
        }

        /**
         * Get object value as double.
         *
         * \returns Value as double
         */
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

        /**
         * Get object value as double.
         *
         * \returns Value as double
         */
        inline operator double () const
        {
            return asDouble();
        }

        /**
         * Check if type of value is array
         *
         * \returns True if type is array.
         */
        inline bool isArray () const
        {
            return (type == json_array);
        }

        /**
         * Check if type of value is object
         *
         * \returns True if type is object.
         */
        inline bool isObject () const
        {
            return (type == json_object);
        }

        /**
         * Check if type of value is string
         *
         * \returns True if type is string.
         */
        inline bool isString () const
        {
            return (type == json_string);
        }

        /**
         * Check if type of value is numeric
         *
         * \returns True if type is numeric.
         */
        inline bool isNumeric () const
        {
            return (type == json_integer || type == json_uinteger || type == json_double);
        }

        /**
         * Check if type of value is double
         *
         * \returns True if type is double.
         */
        inline bool isDouble () const
        {
            return (type == json_double);
        }

        /**
         * Check if type of value is integral
         *
         * \returns True if type is integral.
         */
        inline bool isIntegral () const
        {
            return (type == json_integer || type == json_uinteger);
        }

        /**
         * Check if array has a member with given key name
         *
         * \param key Name of the key
         * \returns True if value for key is found
         */
        inline bool isMember (const std::string &key) const
        {
            if (type != json_object) return false;

            for (unsigned int i = 0; i < u.object.length; ++i) {
                if (u.object.values [i].name == key) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Get a member with given key name
         *
         * \param key Name of the key
         * \returns Value with given name or nullptr
         */
        inline const struct _json_value *getMember (const std::string &key) const
        {
            if (type != json_object) return nullptr;

            for (unsigned int i = 0; i < u.object.length; ++i) {
                if (u.object.values [i].name == key) {
                    return u.object.values[i].value;
                }
            }

            return nullptr;
        }

        /**
         * Get list of member names in object.
         *
         * \returns Vector of strings containing me,ber names
         */
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

/**
 * Parse given JSON string.
 * Parses string and returns object.
 *
 * \param json JSON data as char array
 * \param length Length of the char array
 * \returns Parsed JSON value
 */
json_value * json_parse (const json_char * json,
                         size_t length);

#define json_error_max 128
/**
 * Extended parse of given JSON string.
 * Parses string and returns object.
 * Special settings may be given, and error is returned.
 *
 * \param settings Special settings for parsing
 * \param json JSON data as char array
 * \param length Length of the char array
 * \param error Char array where to copy possible error
 * \returns Parsed JSON value
 */
json_value * json_parse_ex (json_settings * settings,
                            const json_char * json,
                            size_t length,
                            char * error);

/**
 * Frees a JSON value.
 * All values returned by json_parse must be freed
 * in order not to leak memory.
 *
 * \param value Value to be freed.
 */
void json_value_free (json_value *value);


/**
 * Extended free.
 * Not usually necessary, unless you used a custom mem_alloc and now want to
 * use a custom mem_free.
 *
 * \param settings Special settings for parsing
 * \param value Value to be freed.
 */
void json_value_free_ex (json_settings * settings,
                         json_value *value);


#ifdef __cplusplus
   } /* extern "C" */
#endif

#endif


