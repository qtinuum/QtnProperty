# To use *.pef files in your project you should:
# 1. Include this PEG.pri file to your project file
# 2. List your *.pef files in PEG_SOURCES variable
# 3. Define PEG_TOOL to point to QtnPEG before including
# PEG.pri file in your pro file

isEmpty(PEG_TOOL) {
macx:PEG_TOOL = $$PWD/bin-osx/QtnPEG
else:win32:PEG_TOOL = $$PWD/bin-win/QtnPEG.exe
else:linux:PEG_TOOL = $$PWD/bin-linux/QtnPEG
else:PEG_TOOL = QtnPEG
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
peg_header.commands = $$escape_expand(\\n) # empty command
peg_header.CONFIG += target_predeps no_link
peg_header.variable_out = PEG_MOC_HEADERS
QMAKE_EXTRA_COMPILERS += peg_header

load(moc)
peg_moc.name = my_moc_header
peg_moc.output = $$moc_header.output
peg_moc.input = PEG_MOC_HEADERS
peg_moc.commands = $$moc_header.commands
peg_moc.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += peg_moc
