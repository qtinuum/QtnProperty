/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyEnumGenerator.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QRegExp>

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

    PEG& peg = PEG::instance();

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
