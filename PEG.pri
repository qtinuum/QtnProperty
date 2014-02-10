# To use *.pef files in your project you should:
# 1. Include this PEG.pri file to your project file
# 2. List your *.pef files in PEG_SOURCES variable
# 3. Define PEG_TOOL to point to QtnPEG before including
# PEG.pri file in your pro file

isEmpty(PEG_TOOL) {
PEG_TOOL = QtnPEG
}

peg.name = PropertyEnum generator ${QMAKE_FILE_IN}
peg.input = PEG_SOURCES
peg.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.peg.cpp
peg.commands = $$PEG_TOOL ${QMAKE_FILE_IN}
peg.CONFIG += target_predeps
peg.variable_out = SOURCES
silent:peg.commands = @echo PEG ${QMAKE_FILE_IN} && $$peg.commands
QMAKE_EXTRA_COMPILERS += peg

peg_header.name = PEG Headers ${QMAKE_FILE_IN}
peg_header.input = PEG_SOURCES
peg_header.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.peg.h
peg_header.commands = @true
peg_header.CONFIG += target_predeps no_link
peg_header.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += peg_header
