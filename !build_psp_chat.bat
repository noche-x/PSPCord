PATH=C:\pspsdk\bin;%cd%;
make all
move psp-chat.elf output\PSP-Chat
move psp-chat.prx output\PSP-Chat
move PARAM.SFO output\PSP-Chat
move EBOOT.PBP output\PSP-Chat
pause
cd output\PSP-Chat
EBOOT.PBP