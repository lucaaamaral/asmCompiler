#include <stdio.h>
#include <stdint.h>


#define DEF_INPUTFILE "input.asm"
#define DEF_OUTPUTFILE "out.hex"
#define MAX_LINE_LEN 40
#define BIN_MAX_VAL ((uint16_t)(~0)>>1)+1
#define HEX_MAX_VAL ((uint16_t)(~0)>>4)+1
#define DEC_MAX_VAL ((uint16_t)(~0))/10+1
#define REG_MAX_ADDR 0xF
#define REG_MAX_VAL 0xFF
#define PROG_MAX_ADDR 0xFFF

bool openfile(char* inputfile, FILE **file, char *op)
{
    printf("[OPENFILE] Opening file '%s' in the '%s' mode\n", inputfile, op);
    FILE *tfile = (FILE*)malloc(MAX_LEN_FILENAME*sizeof(FILE));
    tfile = fopen(inputfile, op);

    if (tfile==NULL) 
    {
        printf("[OPENFILE] File %s not found.\n", inputfile);
        return false;
    }
    else if (feof(tfile))
    {
        printf("[OPENFILE] Broken EOF in file %s.\n", inputfile);
        return false;
    }
    else
    {
        *file = tfile;
        printf("[OPENFILE] Successfully exited\n");
        return true;  
    }  
}

bool cmpop(char *op, const char *cmp)
{
    int i=0;
    while(cmp[i]!='\0')
    {
        if(op[i]!=cmp[i]) return false;
        i++;
    }
    return true;
}


void skipSpaces (char **instr)
{
    int i=0;
    if (*(*instr+i) == ' ') do { i++; } while(*(*instr+i) == ' ');
    *instr +=i;
    return;
}

bool getval(char **instr, uint16_t *val)
{
    *val = 0;
    int i;
    skipSpaces(instr);
    if (**instr=='0')
    {
        switch (*(*instr+1))
        {
        case 'b':
            i=2;
            while(*(*instr+i)!=' ' && *(*instr+i)!='\n')
            {   
                if ('0'<=*(*instr+i) && *(*instr+i)<='1' && *val<BIN_MAX_VAL)
                {
                    *val<<=1;
                    *val |= (*(*instr+i)-'0');
                    i++;
                }
                else 
                {
                    printf("[GETVAL][bin] Wrong value inserted, review you code\n");
                    return false;
                }
            }
            *instr += i;
            return true;
            break;
        case 'x':
            i=2;
            while(*(*instr+i)!=' ' && *(*instr+i)!='\n')
            {
                if (*val<HEX_MAX_VAL)
                {
                    if ('0'<=*(*instr+i) && *(*instr+i)<='9')
                    {
                        *val<<=4;
                        *val|=(*(*instr+i)-'0');
                        i++;
                    }
                    else if ('A'<=*(*instr+i) && *(*instr+i)<='F')
                    {
                        *val<<=4;
                        *val|=(*(*instr+i)-'A'+10);
                        i++;
                    }
                    else if ('a'<=*(*instr+i) && *(*instr+i)<='f')
                    {
                        *val<<=4;
                        *val|=(*(*instr+i)-'a'+10);
                        i++;
                    }
                    else 
                    {
                        printf("[GETVAL][hex] Wrong value inserted, review you code\n");
                        return false;
                    }
                }
                else 
                {
                    printf("[GETVAL][hex] Wrong value inserted, review you code\n");
                    return false;
                }
            }
            *instr += i;
            return true;
            break;
        
        case 'd':
            i=2;
            while(*(*instr+i)!=' ' && *(*instr+i)!='\n')
            {
                if ('0'<=*(*instr+i) && *(*instr+i)<='9' && *val<DEC_MAX_VAL)
                {
                    *val *= 10;
                    *val += (*(*instr+i)-'0');
                    i++;
                }
                else 
                {
                    printf("[GETVAL][dec] Wrong value inserted, review you code\n");
                    return false;
                }
            }
            *instr += i;
            return true;
            break;
        default:
            i=1;
            while(*(*instr+i)!=' ' && *(*instr+i)!='\n')
            {
                if ('0'<=*(*instr+i) && *(*instr+i)<='9' && *val<DEC_MAX_VAL)
                {
                    *val *= 10;
                    *val+=(*(*instr+i)-'0');
                    i++;
                }
                else 
                {
                    printf("[GETVAL][dec-0] Wrong value inserted, review you code\n");
                    return false;
                }
            }
            *instr += i;
            return true;
            break;
        }
    }
    i=0;
    while(*(*instr+i)!=' ' && *(*instr+i)!='\n')
    {
        if ('0'<=*(*instr+i) && *(*instr+i)<='9' && *val<DEC_MAX_VAL)
        {
            *val *= 10;
            *val+=(*(*instr+i)-'0');
            i++;
        }
        else 
        {
            printf("[GETVAL][dec-def] Wrong value inserted, review you code\n");
            return false;
        }
    }

    *instr += i;
    return true;
}

bool tomachine(char *instr, uint16_t *code)
{
    uint16_t val;
    int i=0;
    *code = 0;
    skipSpaces(&instr); // skip spaces in the begining
    printf("[TOMACHINE] Starting binary conversion of received instruction: %s", instr);

    if (cmpop(instr, "ADDi"))
    {
        instr = &instr[4];
        *code |= 2 << 12; 
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][ADDi] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_VAL)
        {   // break if out of range
            printf("[TOMACHINE][ADDi] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        
        return true;
    }
    else if (cmpop(instr, "ADD"))
    {
        instr = &instr[3];
        *code |= 0 << 12; 
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][ADD] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        
        if (!getval(&instr, &val)) return false;
        
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][ADD] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<4;
        return true;
    }
    else if (cmpop(instr, "SUBi"))
    {
        instr = &instr[4];
        *code |= 3 << 12; 
        
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][SUBi] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_VAL)
        {   // break if out of range
            printf("[TOMACHINE][SUBi] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        
        return true;
    }
    else if (cmpop(instr, "SUB"))
    {
        instr = &instr[3];
        *code |= 1 << 12;
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][SUB] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        
        if (!getval(&instr, &val)) return false;
        
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][SUB] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<4; 
        return true;
    }
    else if (cmpop(instr, "MUL2"))
    {
        instr = &instr[4];
        *code |= 4 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][MUL2] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "DIV2"))
    {
        instr = &instr[4];
        *code |= 5 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][DIV2] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "CLR"))
    {
        instr = &instr[3];
        *code |= 6 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][CLR] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "RST"))
    {
        instr = &instr[3];
        *code |= 7 << 12; 
        return true;
    }
    else if (cmpop(instr, "MOV"))
    {
        instr = &instr[3];
        *code |= 8 << 12; 
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][MOV] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        
        if (!getval(&instr, &val)) return false;
        
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][MOV] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<4; 
        return true;
    }
    else if (cmpop(instr, "JMP"))
    {
        instr = &instr[3];
        *code |= 9 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>PROG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][JMP] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        return true;
    }
    else if (cmpop(instr, "CMP"))
    {
        instr = &instr[3];
        *code |= 10 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][CMP] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "JZ"))
    {
        instr = &instr[2];
        *code |= 11 << 12; 

        if (!getval(&instr, &val)) return false;
    
        if (val>PROG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][STOREM] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        return true;
    }
    else if (cmpop(instr, "INC"))
    {
        instr = &instr[3];
        *code |= 12 << 12; 
        
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][INC] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "DEC"))
    {
        instr = &instr[3];
        *code |= 13 << 12; 
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][DEC] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;
        return true;
    }
    else if (cmpop(instr, "LOADM"))
    {
        instr = &instr[5];
        *code |= 14 << 12; 
        
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][LOADM] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_VAL)
        {   // break if out of range
            printf("[TOMACHINE][LOADM] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        
        return true;
    }
    else if (cmpop(instr, "STOREM"))
    {
        instr = &instr[6];
        *code |= 15 << 12; 
        
        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_ADDR)
        {   // break if out of range
            printf("[TOMACHINE][STOREM] Register %d out of range\n", val);
            return false;
        }
        *code |= val<<8;

        if (!getval(&instr, &val)) return false;
    
        if (val>REG_MAX_VAL)
        {   // break if out of range
            printf("[TOMACHINE][STOREM] Value %d out of range\n", val);
            return false;
        }
        *code |= val;
        
        return true;
    }
    else
    {
        *instr = 0;
    }
    return true;
}

bool write_hex(FILE *asmfile, FILE *outfile)
{
    printf("[WRITEHEX] Begining transcription\n");
    char instr[MAX_LINE_LEN];
    uint16_t code;
    int i=0;
    while (fgets(instr, MAX_LINE_LEN, asmfile)!=NULL)
    {
        i++;
        if(!tomachine(instr, &code))
        {
            printf("[WRITEHEX] Compilation failed at line %d", i);
            return false;
        }
        if (*instr!=0) 
        {
            printf("[WRITEHEX] Code to write: %d\n", code);
            fputc(code>>8, outfile);
            fputc(code, outfile);
        }
    }
    return true;
}

int compile(char *inputfile, char *outputfile)
{
    if (inputfile == NULL) inputfile = (char*)DEF_INPUTFILE;
    if (outputfile == NULL) outputfile = (char*)DEF_OUTPUTFILE;
    printf("[COMPILE] InputFile set as: %s\n", inputfile);
    printf("[COMPILE] OutputFile set as: %s\n", outputfile);

    FILE *asmfile;
    bool error = openfile(inputfile, &asmfile, (char*)"r");
    if (!error) return 404;

    FILE *outfile;
    error = openfile(outputfile, &outfile, (char*)"w");
    if (!error) return 404;

    write_hex(asmfile, outfile);

    free(asmfile);
    free(outfile);
    return 0;
}
