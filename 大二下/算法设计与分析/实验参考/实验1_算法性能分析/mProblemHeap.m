result=[];
for power=1:1
    scale=power*1000000000;
    count=0;
    for times=1:20
        numbers=randi(scale,1,scale);
        number=numbers(1,1:10);
        tic;
        for i=5:-1:1
            number=Adjust(i,10,number);
        end
        for i=11:scale
            if numbers(i)>number(1)
                number(1)=numbers(i);
                number=Adjust(1,10,number);
            end
        end      
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end
function[number]=Adjust(i,m,number)
    temp=number(i);
    j=2*i;
    while j<=m
        if j<m&&number(j)>number(j+1)
            j=j+1;
        end
        if number(j)>=temp
            break
        end
        number(i)=number(j);
        i=j;
        j=j*2;
    end
    number(i)=temp;
end
% function[number]=HeapSort(number,scale)
%     for i=scale:-1:2
%         temp=number(1);
%         number(1)=number(i);
%         number(i)=temp;
%         number=Adjust(1,i-1,number);
%     end
% end