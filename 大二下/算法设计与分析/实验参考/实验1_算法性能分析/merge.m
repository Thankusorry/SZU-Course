result=[];
for power=1:5
    scale=power*100000;
    count=0;
    for times=1:20
        number=randi(scale,1,scale);
        done=zeros(1,scale);
        tic;
        step=1;
%         number=MergeSort(1,scale,number,done);
        while step<scale
            for low=1:2*step:scale
                mid=low+step-1;
                if mid>scale
                    break
                end
                high=low+2*step-1;
                if high>scale
                    high=scale;
                end
                done=Merge(low,mid,high,number,done);
            end
            step=step*2;
            number=done;
        end
        count=count+toc;           
    end
    count=count/20;
    result=[result,count];
end
% function[number]=MergeSort(low,high,number,done)
%     if low<high
%         mid=floor((low+high)/2);
%         number=MergeSort(low,mid,number,done);
%         number=MergeSort(mid+1,high,number,done);
%         number=Merge(low,mid,high,number,number);
%     end
% end
function[done]=Merge(low,mid,high,number,done)
    i=low;
    j=mid+1;
    k=low;
    while i<=mid&&j<=high
        if number(i)<=number(j)
            done(k)=number(i);
            i=i+1;
        else
            done(k)=number(j);
            j=j+1;
        end
        k=k+1;
    end
    while i<=mid
        done(k)=number(i);
        k=k+1;
        i=i+1;
    end
    while j<=high
        done(k)=number(j);
        k=k+1;
        j=j+1;
    end
end