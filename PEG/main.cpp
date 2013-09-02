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

#include "PropertyEnumGenerator.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QRegExp>

using namespace Qtinuum;

extern int yyparse ();
extern FILE *yyin;
extern int yydebug;

int main(int argc, char *argv[])
{
    //yydebug = 1;

    // Qt application
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    // check argument
    if (argc < 2 || argc > 4)
    {
        QTextStream(stdout) << PEG::tr("peg usage: peg <input file> [<cpp output file> [h output file]]") << endl;
        return 0;
    }

    // generate output file names
    const char* inputFileName = argv[1];
    QFileInfo fi(inputFileName);
    if (!fi.exists())
    {
        QTextStream(stderr) << PEG::tr("Error: file '%1' doesn't exist").arg(fi.absoluteFilePath()) << endl;
        return 1;
    }

    QString cppFileName;
    if (argc < 3)
    {
        cppFileName = fi.path() + "/" + fi.completeBaseName() + ".peg.cpp";
    }
    else
    {
        cppFileName = argv[2];
    }

    QString hFileName;
    if (argc < 3)
    {
        hFileName = fi.path() + "/" + fi.completeBaseName() + ".peg.h";
    }
    else if (argc < 4)
    {
        hFileName = cppFileName;
        hFileName.replace(QRegExp(".cpp$"), ".h");
    }
    else
    {
        hFileName = argv[3];
    }

    PEG &peg = PEG::instance();

    if (!peg.start(hFileName, cppFileName))
        return 1;

    // open source file for reading
    FILE* input_file = fopen(inputFileName, "r");
    if (!input_file)
    {
        QTextStream(stderr) << PEG::tr("Error: cannot open file '%1' for reading").arg(inputFileName) << endl;
        return 1;
    }

    yyin = input_file;

    int result = yyparse();

    yyin = stdin;
    fclose(input_file);

    peg.stop();

    return result;
}
