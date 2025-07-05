result=[];
for power=1:1
    scale=power*1000000000;
    count=0;
    for times=1:20
        numbers=randi(scale,1,scale);
        number=numbers(1,1:10);
        tic;
        for i=1:9
            for j=i+1:-1:2
                if number(j)<number(j-1)
                    break
                else
                    temp=number(j);
                    number(j)=number(j-1);
                    number(j-1)=temp;
                end
            end
        end
        for i=11:scale
            for j=10:-1:1
                if number(j)>=numbers(i)
                    break
                else
                    if j<10
                        number(j+1)=number(j);
                    end
                    number(j)=numbers(i);
                end
            end
        end      
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end