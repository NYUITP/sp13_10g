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


#pragma once

#include "select_statement_parser.h"
#include "insert_statement_parser.h"

class SqlStatement {
public:
	typedef enum {
		SELECT =0,
		INSERT =1
	}StatementType;

	typedef union{
		SelectStatement* mSelect;
		InsertStatement* mInsert;
	}Statement;

	SqlStatement (SelectStatement* selectt) : mType(SELECT)
	{
		mStatement.mSelect = selectt;
	}

	SqlStatement (InsertStatement* insertt) : mType(INSERT)
	{
		mStatement.mInsert = insertt;
	}

	 ~SqlStatement()
	{
		switch(mType)
		{
		  case SELECT:
			delete mStatement.mSelect;
			break;
     
		  case INSERT:
			delete mStatement.mInsert;
			break;

		  default:
			std::cout << "Rot in hell!" << std::endl;
		}
    }

	void show()
	{
		switch(mType){
		case SELECT:
			mStatement.mSelect->show();
			break;

		case INSERT:
			mStatement.mInsert->show();
			break;

		default:
			std::cout << "Go to hell!" << std::endl;
    }
  }

private:
	StatementType mType;
	Statement mStatement;

};