result=[];
for power=1:10
    scale=power*100000;
    count=0;
    for times=1:20
        x=randi(scale,1,scale);
        y=randi(scale,1,scale);
        tic;
        [i,j]=brute(x,y,scale);
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end
function [mini,minj]=brute(x,y,scale)
    mini=1;
    minj=1;
    minDistance=Inf;
    for i=1:scale-1
        for j=1:scale
            if i==j
                break
            end
            distance=(x(i)-x(j))^2+(y(i)-y(j))^2;
            if distance<minDistance
                mini=i;
                minj=j;
                minDistance=distance;
            end
        end
    end
end