K:
cd K:\PSP\GAME\
mkdir PSP-Chat
cd PSP-Chat
mkdir data

C:
cd output\PSP-Chat
copy EBOOT.PBP K:\PSP\GAME\PSP-Chat
copy pspchatter.elf K:\PSP\GAME\PSP-Chat
copy pspchatter.prx K:\PSP\GAME\PSP-Chat

cd data
for /r %%v in (*) do copy "%%v" K:\PSP\GAME\PSP-Chat\data\