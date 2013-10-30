/*
 * Copyright (c) 2012 - 2013, the Qtinuum project.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <qtinuum.team@gmail.com>
 */

#ifndef PROPERTY_ENUM_GENERATOR_COMMON_H
#define PROPERTY_ENUM_GENERATOR_COMMON_H

#include <QString>
#define YYSTYPE QString

struct YYLTYPE;

extern int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param);

#endif // PROPERTY_ENUM_GENERATOR_COMMON_H
