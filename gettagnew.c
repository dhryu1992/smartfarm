#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>           // printf 함수, gets_s 함수를 사용하기 위해!
#include <string.h>          // strlen 함수를 사용하기 위해!
#include <malloc.h>          // malloc 함수, free 함수를 사용하기 위해!
#include <stdlib.h>
#define MAX_LENGTH     1025   // 최대 128자까지만 입력 받는다.

typedef char TElement;
typedef struct elementNode{
    TElement data;
    TElement aValue;
    struct eNode* fChild;
    struct eNode* lChild;
    struct eNode* parent;
    struct eNode* attribute;
}eNode;

int main()
{    
    char buffer[MAX_LENGTH] = {0, };
    FILE *fp = fopen("test.html","r");
    if (fp == NULL) {
        fclose(fp);
        return 1;
}

    char *sptr, *eptr, *index;;
    char *temp = malloc(sizeof(char)*MAX_LENGTH);
    char *tag = malloc(sizeof(char)*MAX_LENGTH);
    char *tagname = malloc(sizeof(char)*MAX_LENGTH);
    char *context = malloc(sizeof(char)*MAX_LENGTH);
    char *text = malloc(sizeof(char)*MAX_LENGTH);
    char *attname = malloc(sizeof(char)*MAX_LENGTH);
    char *value = malloc(sizeof(char)*MAX_LENGTH);
 
    *context = 0; 
    *text = 0;
    *tag = 0;
    *tagname = 0;
    *attname = 0;
    *value = 0;
    *temp = 0;
    while((feof(fp)) == 0)  //파일 끝까지 1바이트씩 buffer로 읽어온 후 context로 이동
    {   
        fread(&buffer,sizeof(char),1,fp);
        if(feof(fp)==0)
            strcat(context,buffer);
    }

    sptr=context;
    index=&sptr[1];
    eptr = &index[0];
    int i = 0;
    while(strncmp(index,"\0",1)!=0){
        
        index=&index[1]; 

        if(strncmp(index,">",1)==0){//<태그>태그 확인
            eptr=&index[0];
            memcpy(tag, ++sptr, eptr - sptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
            tag[eptr - sptr] = 0;
            printf("%s\n",tag);
            index=&index[1];
            // if(strcmp(tag,"head")==0){
            //     while(1){
            //         index++;
            //         if(strncmp(index,"<",1)==0){
            //             sptr=index;
            //             if(strncmp(index,">",1)==0)
            //             eptr=index;
            //             memcpy(tagname, sptr, eptr - sptr);  
            //             tagname[eptr - sptr] = 0;
            //             }
                    
            //     }
            // }

            if(strcmp(tag,"body")==0){//바디 태그 내용 추출
                int isspace = 0;
                int i = 0;
                index=&index[1];//<body>에서 포인터를 한칸만 이동시 \n, 공백으로 출력
                while(1){
                    index=&index[1];
                    
                    if(strncmp(index," ",1)!=0 && isspace==0){
                        sptr=&index[0];
                        isspace = 1;

                        if(strncmp(index,"<",1)!=0){//텍스트+태그 출력, 12345<br>
                            while(strncmp(index,"\n",1)!=0){//전부 출력후 개행문자 확인시까지 반복
                                index=&index[1];
                                if(strncmp(index,"<",1)==0){//텍스트 출력
                                    eptr=&index[0];
                                    memcpy(text, sptr, eptr - sptr);  
                                    text[eptr - sptr] = 0;
                                    printf("%s\n",text);
                                    
                                }
                                else if(strncmp(index,">",1)==0){//태그 출력
                                    memcpy(tagname, eptr+1, index-eptr);
                                    tagname[(index-1) - eptr] = 0;
                                    printf("%s\n",tagname);
                                    
                                    break;
                                }
                            }
                        }

                        else if(strncmp(index,"<",1)==0){//태그 내부 출력
                            eptr=&index[0];
                            while(1){
                                index=&index[1];
                                if(strncmp(index," ",1)==0){
                                    if(strncmp(eptr,"=",1)==0){
                                        memcpy(value, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                        value[index - eptr] = 0;
                                        eptr=&index[0];
                                        printf("%s\n",value);//밸류값
                                        
                                    }
                                    else{
                                    memcpy(temp, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                    temp[index - eptr] = 0;
                                    eptr=&index[0];
                                    strcpy(tagname,temp);
                                    printf("%s\n",tagname);//시작태그 네임(img)
                                    }
                                    //속성, 밸류값 = 기준으로 분할하기
                                    
                                }
                                else if(strncmp(index,"=",1)==0){
                                    memcpy(attname, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                    attname[index - eptr] = 0;
                                    printf("%s\n",attname);
                                    eptr=&index[0];
                                    
                                }
                                // else if(strncmp(index,"!",1)==0){
                                //     printf("find !");
                                //     while(strncmp(index,">",1)!=0){
                                //         index=&index[1]; 
                                //         break;
                                //     }
                                // }
                                else if(strncmp(index,">",1)==0){
                                    memcpy(temp, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                    temp[index - eptr] = 0;
                                    eptr=&index[0];
                                    //열림 태그 끝낫을때 텍스트</태그> 구분
                                    
                                    
                                    if(strcmp(temp,"/body")==0){
                                        strcpy(tag,temp);
                                        printf("%s\n",tag);
                                        
                                        break;
                                    }
                                    else if(strcmp(temp,"/body")!=0){
                                        strcpy(value,temp);
                                        printf("%s\n",value);
                                        
                                        if(strncmp(index+1,"\n",1)!=0){
                                            while(1){
                                                index=&index[1];
                                                if(strncmp(index,"<",1)==0){
                                                    memcpy(temp, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                                    temp[index - eptr] = 0;
                                                    printf("%s\n",temp);
                                                    eptr=&index[0];
                                                    
                                                }
                                                else if(strncmp(index,">",1)==0){
                                                    memcpy(tagname, ++eptr, index - eptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                                                    tagname[index - eptr] = 0;
                                                    printf("%s\n",tagname);
                                                    
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        if(strncmp(index,"\n",1)==0){//태그 없이 텍스트만 출력
                            eptr=&index[0];
                            memcpy(text, sptr, eptr - sptr);         //바디태그의 끝 >에서 한칸 옮긴 H ~ <까지 text에 넣음
                            text[eptr - sptr] = 0;
                            printf("%s\n",text);
                            isspace=0;
                        }
                    }
                    if(strncmp(index,"\n",1)==0){
                        isspace=0;
                        if(strcmp(tag,"/body")==0){
                            break;
                        }
                    }               
                }
            }
        }
        else if(strncmp(index,"<",1)==0){// <검출 위의 if문과 합쳐져서 태그 획득
            sptr=&index[0];
        }       
    }
    free(context);
    free(tag);
    free(text);
    free(tagname);
    free(attname);
    free(temp);
    free(value);
    fclose(fp);
    return 0;    
}