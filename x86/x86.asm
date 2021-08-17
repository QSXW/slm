%define suffix 'slm'

%macro cglobal 1-2+ ""
    cglobal_internal 1, %1 %+ suffix, %2
%endmacro

%macro cglobal_internal 2-3+
    
%endmacro