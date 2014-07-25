# -*- python -*-

from omniidl import idlvisitor, idlast;
from omniidl import output;
import os;
import string;

import simplecxx, identifier;

class Walker(idlvisitor.AstVisitor):
    """Walks over the AST once and writes the header as it goes.
    """
    def visitAST(self, node):
        """Visit all the declarations in an AST"""

        self.masterFile = node.file()
        self.beenIncluded = {}
        self.cxxheader.out("/* This file is automatically generated from " +\
                           node.filename + """
 * DO NOT EDIT DIRECTLY OR CHANGES WILL BE LOST!
 */""")
        self.masterGuard = '';
        for i in node.filebase:
            if (i >= 'a' and i <= 'z') or (i >= 'A' and i <= 'Z'):
                self.masterGuard = self.masterGuard + i
        self.cxxheader.out("#ifndef _GUARD_IFACE_" + self.masterGuard)
        self.cxxheader.out("#define _GUARD_IFACE_" + self.masterGuard)
        self.cxxheader.out('#include "cda_compiler_support.h"')
        self.cxxheader.out('#ifdef MODULE_CONTAINS_' + self.masterGuard)
        self.cxxheader.out('#define PUBLIC_' + self.masterGuard + '_PRE CDA_EXPORT_PRE')
        self.cxxheader.out('#define PUBLIC_' + self.masterGuard + '_POST CDA_EXPORT_POST')
        self.cxxheader.out('#else')
        self.cxxheader.out('#define PUBLIC_' + self.masterGuard + '_PRE CDA_IMPORT_PRE')
        self.cxxheader.out('#define PUBLIC_' + self.masterGuard + '_POST CDA_IMPORT_POST')
        self.cxxheader.out('#endif')
        self.scope = ["iface"]
        self.scopeEntryDepth = 0
        for n in node.declarations():
            if n.file() == self.masterFile:
                n.accept(self)
            else:
                self.considerIncluding(n.file())
        self.escapeScopes()
        self.cxxheader.out('#undef PUBLIC_' + self.masterGuard + '_PRE')
        self.cxxheader.out('#undef PUBLIC_' + self.masterGuard + '_POST')
        self.cxxheader.out("#endif // guard")

    def writeScopes(self):
        for i in range(self.scopeEntryDepth, len(self.scope)):
            self.cxxheader.out('namespace ' + self.scope[i])
            self.cxxheader.out('{')
            self.cxxheader.inc_indent()
        self.scopeEntryDepth = len(self.scope)

    def escapeScopes(self):
        for i in range(0, self.scopeEntryDepth):
            self.cxxheader.dec_indent()
            self.cxxheader.out('};')
        self.scopeEntryDepth = 0

    def enterScope(self, node):
        self.scope.append(node.simplename)

    def leaveScope(self):
        self.scope = self.scope[:-1]
        if self.scopeEntryDepth > len(self.scope):
            self.scopeEntryDepth = len(self.scope)
            self.cxxheader.dec_indent()
            self.cxxheader.out('};')

    def considerIncluding(self, name):
        if (self.beenIncluded.has_key(name)):
            return
        self.beenIncluded[name] = 1;
        self.escapeScopes()
        basename,ext = os.path.splitext(name)
        self.cxxheader.out('#include "Iface' + basename  + '.hxx"')
        self.cxxheader.inModule = 0

    def visitModule(self, node):
        """Visit all the definitions in a module."""
        self.enterScope(node)
        for n in node.definitions():
            if n.file() == self.masterFile:
                self.writeScopes()
                n.accept(self)
            else:
                self.considerIncluding(n.file())
        self.leaveScope()

    def visitForward(self, node):
        self.cxxheader.out('class ' + node.simplename + ';')

    def visitInterface(self, node):
	self.cxxheader.out('PUBLIC_' + self.masterGuard + '_PRE ')
        self.cxxheader.out('class  PUBLIC_' + self.masterGuard + '_POST ' +
                           node.simplename)
        inh = node.inherits()
        if len(inh) > 0:
	    inhs = ''
            needcomma = 0
            for ic in inh:
                while isinstance(ic, idlast.Declarator):
                    ic = ic.alias().aliasType().decl()
                    identifier.AnnotateByRepoID(ic)
                if needcomma:
                    inhs = inhs + ' , '
                needcomma = 1
                inhs = inhs + 'public virtual ' + ic.simplecxxscoped
            self.cxxheader.out(' : ' + inhs)
        self.cxxheader.out('{')
        self.cxxheader.out('public:')
        self.cxxheader.inc_indent()
        # Write a pure virtual destructor...
        self.cxxheader.out('static const char* INTERFACE_NAME() { return "' + node.corbacxxscoped + '"; }')
        self.cxxheader.out('virtual ~' + node.simplename + '() {}')
        for n in node.contents():
            n.accept(self)
        self.cxxheader.dec_indent()
        self.cxxheader.out('};')
    def visitConst(self, node):
        # Note: Can't have a const sequence, so don't call doesTypeNeedLenth
        self.cxxheader.out('static const ' +\
                           simplecxx.typeToSimpleCXX(node.constType()) +\
                           ' ' + node.simplename + ' = ' +\
                           simplecxx.enumOrInt(node.value()) + ';')

    def visitTypedef(self, node):
        alln = ''
        needcomma = 0
        for n in node.declarators():
            if needcomma:
                alln = alln + ', '
            needcomma = 1
            alln = alln + n.simplename
            sa = n.sizes()
            if sa != None:
                for s in sa:
                    alln = alln + '[%u]'%s
        # We always alias to the array part of a sequence.
        alln = 'typedef ' + simplecxx.typeToSimpleCXX(node.aliasType()) +\
               ' ' + alln + ';'
        self.cxxheader.out(alln)

    def visitMember(self, node):
        alln = ''
        needcomma = 0
        for n in node.declarators():
            if needcomma:
                alln = alln + ', '
            needcomma = 1
            alln = alln + n.simplename
            sa = n.sizes()
            if sa != None:
                for s in sa:
                    alln = alln + '[%u]'%s
        alln = simplecxx.typeToSimpleCXX(node.memberType(), is_ret=1) +\
               ' ' + alln + ';'
        self.cxxheader.out(alln)

    def visitStruct(self, node):
        if (node.recursive()):
            self.visitStructForward(node)
        self.cxxheader.out('typedef struct _struct_' + node.simplename)
        self.cxxheader.out('{')
        self.cxxheader.inc_indent()
        for n in node.members():
            n.accept(self)
        self.cxxheader.dec_indent()
        self.cxxheader.out('} ' + node.simplename + ';')

    def visitStructForward(self, node):
        self.cxxheader.out('typedef struct _struct_' + node.simplename +\
                           ' ' + node.simplename + ';')

    def visitException(self, node):
	self.cxxheader.out('PUBLIC_' + self.masterGuard + '_PRE ')
        self.cxxheader.out('class  PUBLIC_' + self.masterGuard + '_POST ' + node.simplename + ' : public std::exception')
        self.cxxheader.out('{')
        self.cxxheader.out('public:')
        self.cxxheader.inc_indent()
        constructorArgs = ''
        constructorSave = ''
        for n in node.members():
            if constructorArgs == '':
                constructorSave = ' : '
            else:
                constructorArgs = constructorArgs + ', '
            for dn in n.declarators():
                constructorSave = constructorSave + ('%s(_%s)' % (dn.simplename, dn.simplename))
                constructorArgs = constructorArgs + simplecxx.typeToSimpleCXX(n.memberType(), is_const=1) +\
                    ' _' + dn.simplename
                if dn.sizes() != None:
                    constructorArgs = constructorArgs + string.join(map(lambda x: '[%s]'%x, dn.sizes()), '')

        self.cxxheader.out('  ' + node.simplename + '(' + constructorArgs + ')' + constructorSave + '{}')
        self.cxxheader.out('  ~' + node.simplename + '() throw() {}')
        for n in node.members():
            n.accept(self)
        self.cxxheader.dec_indent()
        self.cxxheader.out('};')

    def visitCaseLabel(self, node):
        return

    def visitUnionCase(self, node):
        pass # not called.

    def visitUnion(self, node):
        raise "Unions are not supported"
        # We are not supporting unions for now.
        # for n in node.cases():
        #    n.accept(self)

    def visitUnionForward(self, node):
        # AnnotateByRepoID(node)
        raise "Unions are not supported"

    def visitEnumerator(self, node):
        if node.islast:
            comma = ''
        else:
            comma = ','
        self.cxxheader.out(node.simplename + (' = %u'%node.value()) +\
                           comma)

    def visitEnum(self, node):
        isfirst = 1
        self.cxxheader.out('typedef enum _enum_' + node.simplename)
        self.cxxheader.out('{')
        self.cxxheader.inc_indent()
        nel = len(node.enumerators())
        for n in node.enumerators():
            nel = nel - 1
            n.islast = (nel == 0)
            n.accept(self)
        self.cxxheader.dec_indent()
        self.cxxheader.out('} ' + node.simplename + ';')

    def visitAttribute(self, node):
        typename = simplecxx.typeToSimpleCXX(node.attrType(), is_ret=1)
        typenameC = simplecxx.typeToSimpleCXX(node.attrType(), is_const=1)
        possibleWarnUnused = simplecxx.shouldWarnIfUnused(node.attrType());
        for n in node.declarators():
            self.cxxheader.out('virtual ' + typename + ' ' + n.simplename +
                               '(' +
                               ') throw(std::exception&) ' +
                               possibleWarnUnused + ' = 0;')
            if not node.readonly():
                self.cxxheader.out('virtual void ' + n.simplename + '(' + typenameC +
                                   ' attr' +
                                   ') throw(std::exception&) = 0;')

    def visitOperation(self, node):
        rtype = simplecxx.typeToSimpleCXX(node.returnType(), is_ret=1)
        if node.simplename == 'query_interface':
            rtype = 'void*'
        call = 'virtual ' + rtype
        call = call + ' ' + node.simplename + '('
        needcomma = 0
        for n in node.parameters():
            if needcomma:
                call = call + ', '
            needcomma = 1
            if n.is_out():
                numpoint = 1
            else:
                numpoint = 0
            call = call + simplecxx.typeToSimpleCXX(n.paramType(), numpoint,
                                                    not n.is_out()) +\
                   ' ' + n.simplename

        # Every operation can throw, e.g. in an I/O error, not just those that
        # list exceptions with raises.
        call = call + ') throw(std::exception&)' +\
               simplecxx.shouldWarnIfUnused(node.returnType()) + ' = 0;'
        self.cxxheader.out(call)

def run(tree):
    w = Walker()
    w.cxxheader = output.Stream(open("Iface" + tree.filebase + ".hxx", "w"), 2);
    tree.accept(w)
