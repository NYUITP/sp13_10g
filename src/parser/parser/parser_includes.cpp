//  Copyright [2013] Kyle Galloway (kyle.s.galloway@gmail.com)
//                   Pravish Sood (pravish.sood@gmail.com)
//                   Dylan Kelemen (dckelemen@gmail.com)

//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at

//      http://www.apache.org/licenses/LICENSE-2.0

//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


#include "parser_includes.h"

const std::string SqlConstants::lt					= "<"; 
const std::string SqlConstants::gt					= ">";
const std::string SqlConstants::lte					= "<=";
const std::string SqlConstants::gte					= ">=";
const std::string SqlConstants::et					= "="; 
const std::string SqlConstants::net					= "<>";
const std::string SqlConstants::like				= "like"; 
const std::string SqlConstants::between				= "between";
const std::string SqlConstants::distinct			= "distinct"; 
const std::string SqlConstants::all					= "all";
const std::string SqlConstants::select				= "select";
const std::string SqlConstants::insert				= "insert";
const std::string SqlConstants::sqlDelete			= "delete";
const std::string SqlConstants::drop				= "drop";
const std::string SqlConstants::create				= "create";
const std::string SqlConstants::update				= "update";
const std::string SqlConstants::into				= "into";
const std::string SqlConstants::values				= "values";
const std::string SqlConstants::bLiteral_true		= "true";
const std::string SqlConstants::bLiteral_false		= "false";
const std::string SqlConstants::bLiteral_yes		= "yes";
const std::string SqlConstants::bLiteral_no			= "no";
const std::string SqlConstants::bLiteral_on			= "on";
const std::string SqlConstants::bLiteral_off		= "off";