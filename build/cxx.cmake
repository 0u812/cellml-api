SET(CXX_IFACE_HEADER_LIST)
FOREACH(idlfile ${IDL_LIST})
  SET(idlpath "interfaces/${idlfile}.idl")
  SET(headerpath "interfaces/Iface${idlfile}.hxx")

  ADD_CUSTOM_COMMAND(OUTPUT ${headerpath}
                     COMMAND ${OMNIIDL} -bsimple_cpp -Iinterfaces -p../simple_interface_generators/omniidl_be ../${idlpath}
                     MAIN_DEPENDENCY ${idlpath}
                     DEPENDS simple_interface_generators/omniidl_be/simple_cpp/cxxheadergen.py
                             simple_interface_generators/omniidl_be/simple_cpp/__init__.py
                             simple_interface_generators/omniidl_be/simple_cpp/simplecxx.py
                             simple_interface_generators/omniidl_be/simple_cpp/identifier.py
                     WORKING_DIRECTORY interfaces
                     VERBATIM)
  LIST(APPEND CXX_IFACE_HEADER_LIST ${headerpath})
ENDFOREACH(idlfile)

ADD_CUSTOM_TARGET(CXX_HEADERS_FOR_INTERFACES ALL DEPENDS ${CXX_IFACE_HEADER_LIST})
