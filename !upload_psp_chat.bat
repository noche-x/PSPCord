cd K:\PSP\GAME\psp_chatter\
K:

del EBOOT.PBP
del pspchatter.elf
del pspchatter.prx

for /r %%v in (*) do del "%%v"

cd data
for /r %%v in (*) do del "%%v"

cd K:\PSP\GAME\psp_chatter\
mkdir data

C:
cd output
copy EBOOT.PBP K:\PSP\GAME\psp_chatter
copy pspchatter.elf K:\PSP\GAME\psp_chatter
copy pspchatter.prx K:\PSP\GAME\psp_chatter

cd..
cd data
for /r %%v in (*) do copy "%%v" K:\PSP\GAME\psp_chatter\data\