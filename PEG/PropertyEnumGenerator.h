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

#ifndef PROPERTY_ENUM_GENERATOR_H
#define PROPERTY_ENUM_GENERATOR_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QStack>
#include <QSharedPointer>
#include <QTextStream>

class TextStreamIndent: public QTextStream
{
public:
    TextStreamIndent()
        : m_indent(0)
    {
    }

    QTextStream& newLine(int indentOffset = 0)
    {
        if (!m_wrapperLines.isEmpty())
        {
            auto& info = m_wrapperLines.top();
            if (!info.active)
            {
                info.active = true;
                newLine();
                newLine(info.indentOffset) << info.firstLine;
            }
        }

        return *this << endl << QString(TAB_LEN*(m_indent+indentOffset), ' ');
    }

    void addIndent() { ++m_indent; }
    void delIndent() { --m_indent; }

    void pushWrapperLines(const QString& firstLine, const QString& lastLine, int indentOffset = 0)
    {
        WrapperLinesInfo info;
        info.firstLine = firstLine;
        info.lastLine = lastLine;
        info.indentOffset = indentOffset;
        m_wrapperLines.push(info);
    }

    bool popWrapperLines()
    {
        Q_ASSERT(!m_wrapperLines.isEmpty());
        auto info = m_wrapperLines.pop();
        if (info.active && !info.lastLine.isEmpty())
            newLine(info.indentOffset) << info.lastLine;
        return info.active;
    }

private:
    int m_indent;
    static const int TAB_LEN = 4;

    struct WrapperLinesInfo
    {
        QString firstLine;
        QString lastLine;
        int indentOffset;
        bool active;

        WrapperLinesInfo(): indentOffset(0), active(false) {}
    };

    QStack<WrapperLinesInfo> m_wrapperLines;
};

class Code
{
    Q_DISABLE_COPY(Code)

public:
    Code() {}
    virtual ~Code() {}

    virtual void generateHFile(TextStreamIndent& s) const = 0;
    virtual void generateCppFile(TextStreamIndent& s) const = 0;
};

class PEG: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PEG)

public:
    ~PEG() {}

    static PEG& instance();

    bool start(QString hFileName, QString cppFileName);
    void stop();

    void fatalError(QString error);

    QVector<QSharedPointer<Code> > code;

private:
    PEG();

    bool isValid() const { return m_isValid; }
    void printError(QString error);

    void generateOutputs();
    void generateHFile();
    void generateCppFile();

     bool m_isValid;

    QString m_hFileName;
    QString m_hDefine;

    TextStreamIndent m_hStream;
    TextStreamIndent m_cppStream;
    QTextStream m_errStream;
};

QString propertyType(QString type);
extern PEG& peg;

// types
struct AssignInfo
{
    QString member;
    QString value;
};

// key - setter name (without set)
// value - setter parameter
typedef QMap<QString, AssignInfo> Assignments;
// key - slot name
// values - slot code
typedef QMap<QString, AssignInfo> Slots;
typedef QMap<QString, QString> Signatures;
// key - name exception
typedef QSet<QString> Exceptions;

struct DelegateInfo
{
    QString name;
    QMap<QString, QString> attributes;

    void generateCode(TextStreamIndent& s) const;
};

// code for #include constructions
struct IncludeCode: public Code
{
    QString name;
    bool isHeader;

    IncludeCode(QString name, bool isHeader);

    void generateHFile(TextStreamIndent& s) const override;
    void generateCppFile(TextStreamIndent& s) const override;

private:
    void generate(TextStreamIndent& s) const { s.newLine() << "#include " << name; }

    Q_DISABLE_COPY(IncludeCode)
};

// code for plain C++ code
struct SourceCode: public Code
{
    QString code;
    bool code_h;

    SourceCode(const QString& code, bool code_h);

    void generateHFile(TextStreamIndent& s) const override;
    void generateCppFile(TextStreamIndent& s) const override;
};

struct PropertySetMemberCode
{
    PropertySetMemberCode(): _extern(false) {}
    virtual ~PropertySetMemberCode() {}

    QString name;
    QString selfType;
    Assignments assignments;
    Slots slots_code;
    bool _extern;

    QScopedPointer<DelegateInfo> delegateInfo;
};

// code for property
struct PropertyCode: public PropertySetMemberCode
{
    PropertyCode() { _extern = true; }
};

// code for property_set
struct PropertySetCode: public Code, public PropertySetMemberCode
{
    QVector<QSharedPointer<PropertySetMemberCode>> members;
    QVector<QSharedPointer<PropertySetCode>> subPropertySets;
    QVector<QSharedPointer<SourceCode>> sourceCodes;
    QString superType;

    struct Constructor
    {
        QString code;
        QString initialization_list;
    };
    QScopedPointer<Constructor> constructor;

    struct Destructor
    {
        QString code;
    };
    QScopedPointer<Destructor> destructor;


    PropertySetCode();

    void setName(QString _name);
    void setSuperType(const PropertySetCode* parent);
    void setConstructorCode(QString _name,QString _initialization_list, QString _code);
    void setDestructorCode(QString _name, QString _code);

    void generateHFile(TextStreamIndent& s) const override;
    void generateCppFile(TextStreamIndent& s) const override;

private:
    QString constructorParamsList() const;

    void generateChildrenDeclaration(TextStreamIndent& s) const;
    void generateChildrenInitialization(TextStreamIndent& s) const;
    void generateChildrenAssignment(TextStreamIndent& s) const;
    void generateChildrenCopy(TextStreamIndent& s) const;
    void generateSubPropertySetsDeclarations(TextStreamIndent& s) const;
    void generateSubPropertySetsImplementations(TextStreamIndent& s) const;
    void generateSlotsDeclaration(TextStreamIndent& s) const;
    void generateSlotsImplementation(TextStreamIndent& s) const;
    void generateHSourceCode(TextStreamIndent& s) const;
    void generateCppSourceCode(TextStreamIndent& s) const;
    void generateDelegatesConnection(TextStreamIndent& s) const;
    void generateConstructorInitializationList(TextStreamIndent& s) const;
    void generateCopyValues(TextStreamIndent& s) const;

    static QString slotMember(const QString& name);
    static QString slotMember(const QString& name, const QString& subMember);

    void generateSlotsConnections(TextStreamIndent& s, const QString& name) const;
    QVector<const PropertySetCode*> getSubpropertySets() const;
};

// code for enums
struct EnumCode: public Code
{
    QString name;

    struct EnumItemCode
    {
        QString name;
        int id;
        QString text;
        QVector<QString> states;
    };

    QVector<EnumItemCode> items;

    EnumCode(const QString& name);

    void generateHFile(TextStreamIndent& s) const override;
    void generateCppFile(TextStreamIndent& s) const override;
};

struct PegContext
{
    QSharedPointer<PropertySetCode> propertySet;
    QStack<PropertySetCode*> subPropertySets;

    PropertySetCode *currPropertySet;
    PropertyCode *currProperty;

    QString currAssignMember;
    QString currAssignFunc;

    QSharedPointer<EnumCode> enumCode;

    struct TypeInfo
    {
        QString namespaceName;
        QString name;
    };
    TypeInfo currType;

    PegContext();

    PropertySetMemberCode *current();
    QString currentType() const;
    QString currentPropertyType() const;
    QString currentPropertySetType() const;

    void startPropertySet();
    void commitPropertySet();

    void startProperty();
    void commitProperty();

    bool checkSlotName(const QString& name);

private:
    Q_DISABLE_COPY(PegContext)
};
extern PegContext pegContext;


#endif // PROPERTY_ENUM_GENERATOR_H
