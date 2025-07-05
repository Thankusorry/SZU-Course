result=[];
for power=1:5
    scale=power*10000;
    count=0;
    for times=1:20
        number=randi(scale,1,scale);
        tic;
        number=Quick(1,scale,number);
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end
function[number]=Quick(low,high,number)
    i=low;
    j=high;
    pivot=number(low);
    while low<high
        while low<high&&pivot<=number(high)
            high=high-1;
        end
        if low<high
            number(low)=number(high);
            low=low+1;
        end
        while low<high&&pivot>number(low)
            low=low+1;
        end
        if low<high
            number(high)=number(low);
            high=high-1;
        end
    end
    number(low)=pivot;
    if i<low-1
        number=Quick(i,low-1,number);
    end
    if high+1<j
        number=Quick(high+1,j,number);
    end
end