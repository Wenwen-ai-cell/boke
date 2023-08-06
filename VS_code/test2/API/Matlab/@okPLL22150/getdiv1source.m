function y = getdiv1source(obj)

%GETDIV1SOURCE  Get the divider 2 source.
%  Y=GETDIV1SOURCE(OBJ) returns the divider 2 source in Y.
%  The result is a string representing the source:
%     'DivSrc_Ref'
%     'DivSrc_VCO'
%
%  Copyright (c) 2005 Opal Kelly Incorporated
%  $Rev$ $Date$

y = calllib('okFrontPanel', 'okPLL22150_GetDiv1Source', obj.ptr);
