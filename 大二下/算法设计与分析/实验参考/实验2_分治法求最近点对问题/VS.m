result=[];
for power=1:20
    scale=10000;
    number=100+power*10;
    count=0;
    for times=1:30
        x=randi(scale,1,scale);
        y=randi(scale,1,scale);
        tic;
        [x,y]=Quick(1,scale,x,y);
        [mini,minj,minDistance]=divide(x,y,scale,number);
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end
function [mini,minj,minDistance]=brute(x,y,scale)
    mini=1;
    minj=2;
    minDistance=Inf;
    for i=1:scale
        for j=1:scale
            if j==i
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
function [mini,minj,minDistance]=divide(x,y,scale,number)
    if length(x)<number
        [mini,minj,minDistance]=brute(x,y,scale);
        return;
    end
    half=floor(scale/2);
    [i,j,minLeft]=divide(x(1:half),y(1:half),half,number);
    [ii,jj,minRight]=divide(x(half+1:scale),y(half+1:scale),scale-half,number);
    if minLeft<minRight
        minDistance=minLeft;
        mini=i;
        minj=j;
    else
        minDistance=minRight;
        mini=ii;
        minj=jj;
    end
    left=[];
    right=[];
    for i=half-1:-1:1
        if abs(x(i)-x(half))>=sqrt(minDistance)
            break
        end
        left=[left,i];
    end
    for i=half+1:scale
        if abs(x(i)-x(half))>=sqrt(minDistance)
            break
        end
        right=[right,i];
    end
    for i=1:length(left)
        for j=1:length(right)
            if j>3
                break
            end
            distance=(x(left(i))-x(right(j)))^2+(y(left(i))-y(right(j)))^2;
            if distance<minDistance
                mini=left(i);
                minj=right(j);
                minDistance=distance;
            end
        end
    end
    for i=1:length(right)
        for j=1:length(left)
            if j>3
                break
            end
            distance=(x(left(j))-x(right(i)))^2+(y(left(j))-y(right(i)))^2;
            if distance<minDistance
                mini=left(j);
                minj=right(i);
                minDistance=distance;
            end
        end
    end
end
function[x,y]=Quick(low,high,x,y)
    i=low;
    j=high;
    pivot=x(low);
    temp=y(low);
    while low<high
        while low<high&&pivot<=x(high)
            high=high-1;
        end
        if low<high
            x(low)=x(high);
            y(low)=y(high);
            low=low+1;
        end
        while low<high&&pivot>x(low)
            low=low+1;
        end
        if low<high
            x(high)=x(low);
            y(high)=y(low);
            high=high-1;
        end
    end
    x(low)=pivot;
    y(low)=temp;
    if i<low-1
        [x,y]=Quick(i,low-1,x,y);
    end
    if high+1<j
        [x,y]=Quick(high+1,j,x,y);
    end
end