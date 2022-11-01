[BITS   16]
[org    0X7C00]

gdt:

gdt_null:
        dq      0
gdt_code:
        dw      0ffffh
        dw      0
        db      0
        db      10011010b
        db      11001111b
        db      0
        
gdt_data:
        dw      0FFFFh
        dw      0
        db      0
        db      10010010b
        db      11001111b
        db      0
gdt_end:
gtd_desc:
        db      gdt_end                         - gdt
        dw      gdt
