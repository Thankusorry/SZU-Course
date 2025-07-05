result=[];
for power=1:5
    scale=power*100;
    count=0;
    for times=1:20
        number=randi(scale,1,scale);
        tic;
        for i=scale:-1:1
            for j=1:i-1
                if number(j)>number(j+1)
                    temp=number(j);
                    number(j)=number(j+1);
                    number(j+1)=temp;
                end
            end
        end
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end