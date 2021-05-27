/*
                    2020CSM1001
                    AADITYA RAJ BARNWAL

*/

#include<bits/stdc++.h>
using namespace std;

#define REP(i,n) for(int i=0;i<n;i++)    // Its a loop, I have used this "REP" structure


struct Points{                           // I have created a Structure of type Point,its members are x and y coordinates
int x,y;
};
int bucket_size;                         // Its the global variable , the size of the bucket, the only why i have declared it as global , because in this case i dont have to handle the function argument as its global so it can be accessed from any function
int get_random_point()                   // Its for creating the dataset , returns the random points from 0 to 400
{
    return rand()%400;

}


bool sort_it_in_x(Points p,Points q)                // simple sort function to sort the points Data structure in x wise, its useful in calculating the median while splitting
{
    return p.x<q.x;
}
bool sort_it_in_y(Points p,Points q)                // same as above , it sort in y wise
{
    return p.y<q.y;
}

struct Bucket                                       // its a structure of bucket , it contains all the four co-ordinates of the grid,lower left and upper right and it contains a list points which are inside that grid
{
    int lower_left_x,lower_left_y,upper_right_x,upper_right_y;
    vector<Points> Points_inside_this_bucket;
};
vector<Bucket> Bucket_list_vector;                   // global data structure for storing the points in buckets
void create_dataset()                                // its for creating dataset
{

    int records;
    cout<<"Enter the No. of Records \n";    // asking the user to enter the total number of records
    cin>>records;
    string filename="dataset.txt";
    fstream new_file;
    new_file.open(filename,ios::out);
    REP(i,records)
    {
        string line="";
        line.append(to_string(i+1));
        line.push_back(' ');
        line.append(to_string(get_random_point()));
        line.push_back(' ');
        line.append(to_string(get_random_point()));
        new_file<<line;
        new_file<<"\n";


    }

}
vector<Points> total_number_points_inside_the_grid;
pair<int,int> fetch_x_and_y(string line)              // it return the x and y coordinates of the point while reading from the dataset file
{
    pair<int,int> xy;
    vector <string> tokens;


    stringstream check1(line);

    string intermediate;


    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    xy.first=stoi(tokens[1]);
    xy.second=stoi(tokens[2]);
    return xy;


}
map<string,int>my_map;                                // A map which note the number of points , and whether its repeating or not
void calculate_the_total_points_which_are_inside_the_grid(int lower_left_x,int upper_right_x,int lower_left_y,int upper_right_y)        // this function calculate the number of points which are inside the given grid and reading from dataset file
{
    string filename="dataset.txt";
    my_map.clear();
    fstream new_file;
    new_file.open(filename,ios::in);
    string line;
    while(getline(new_file,line))
    {
    pair<int,int>xy=fetch_x_and_y(line);

    if (( xy.first>=lower_left_x &&  xy.first<=upper_right_x) && (xy.second>=lower_left_y && xy.second<=upper_right_y))

       {
         Points p;
         p.x=xy.first;
         p.y=xy.second;
         total_number_points_inside_the_grid.push_back(p);
         string f=to_string(p.x)+" "+to_string(p.y);
         my_map[f]++;
           // map becaz to find the repeated points
       }

    }
new_file.close();

}
unordered_set<int> X_Scales,Y_Scales;     // it will store the splits value of x and y,
bool check_if_this_points_lie_in_the_coordinates(Points p,int lower_left_x,int lower_left_y,int upper_right_x,int upper_right_y)          // it return true if the point is lie in the given range
{
    int curr_x=p.x;
    int curr_y=p.y;
    if(curr_x>=lower_left_x && curr_x<=upper_right_x && curr_y<=upper_right_y && curr_y>=lower_left_y)
        return true;
    return false;

}

bool check_for_feasible_divide_along_x_direction(int lx,int ly,int k,int ry,int rx,int index,Points element)                 // this function check whether the prev x split help in this bucket or not,if it help then return true and proceed as x split else return false
{
        int left_lower_x=lx,left_lower_y=ly,left_upper_x=k,left_upper_y=ry,right_lower_x=k,right_lower_y=ly,right_upper_x=rx,right_upper_y=ry;

        vector<Points> first,second,temp=Bucket_list_vector[index].Points_inside_this_bucket;
        temp.push_back(element);
        for(Points p:temp)
        {
            int curr_x=p.x;
            int curr_y=p.y;
            if(curr_x>=left_lower_x &&curr_x<=left_upper_x &&curr_y<=left_upper_y && curr_y >=left_lower_y &&first.size()<bucket_size)
            first.push_back(p);
            else
                second.push_back(p);
        }

        if(first.size()<=bucket_size && second.size()<=bucket_size)   // condition so that all bucket has element less then bucket size
        {
            Bucket_list_vector[index].lower_left_x=left_lower_x;
            Bucket_list_vector[index].lower_left_y=left_lower_y;
            Bucket_list_vector[index].upper_right_x=left_upper_x;
            Bucket_list_vector[index].upper_right_y=left_upper_y;
            Bucket_list_vector[index].Points_inside_this_bucket=first;

            Bucket b;
            b.lower_left_x=right_lower_x;
            b.lower_left_y=right_lower_y;
            b.upper_right_x=right_upper_x;
            b.upper_right_y=right_upper_y;
            b.Points_inside_this_bucket=second;
            Bucket_list_vector.push_back(b);


            return true;
        }
        return false;



}


bool check_for_feasible_divide_along_y_direction(int lx,int ly,int k,int ry,int rx,int index,Points element)   // this is the same as above function , it does that in y split
{
     vector<Points> first,second,temp=Bucket_list_vector[index].Points_inside_this_bucket;
     temp.push_back(element);

     int lower_side_lx=lx,lower_side_ly=ly,lower_side_rx=rx,lower_side_ry=k,upper_side_lx=lx,upper_side_ly=k,upper_side_rx=rx,upper_side_ry=ry;

     for(Points p:temp)
     {
          int curr_x=p.x;
          int curr_y=p.y;
          if(curr_x>=lower_side_lx &&curr_x<=lower_side_rx &&curr_y<=lower_side_ry && curr_y >=lower_side_ly && first.size()<bucket_size)
            first.push_back(p);
            else
                second.push_back(p);
     }
     if(first.size()<=bucket_size && second.size()<=bucket_size)
        {
            Bucket_list_vector[index].lower_left_x=lower_side_lx;
            Bucket_list_vector[index].lower_left_y=lower_side_ly;
            Bucket_list_vector[index].upper_right_x=lower_side_rx;
            Bucket_list_vector[index].upper_right_y=lower_side_ry;
            Bucket_list_vector[index].Points_inside_this_bucket=first;

            Bucket b;
            b.lower_left_x=upper_side_lx;
            b.lower_left_y=upper_side_ly;
            b.upper_right_x=upper_side_rx;
            b.upper_right_y=upper_side_ry;
            b.Points_inside_this_bucket=second;
            Bucket_list_vector.push_back(b);
            return true;
        }

return false;


}

int find_the_median_in_x(vector<Points> temp)        // find the median in x
{
    int x;
    int n=temp.size();
    if(temp.size()%2==0) // even number of points are there
    {
        x=(temp[n/2].x+temp[(n/2)-1].x)/2;
    }
    else
    {
        x=temp[(n-1)/2].x;
    }
    return x;

}

int find_the_median_in_y(vector<Points> temp)        // find the median in y
{

    int y,n=temp.size();
    if(n%2==0)
    {
        y=(temp[n/2].y+temp[(n/2)-1].y)/2;
    }
    else
        y=temp[(n-1)/2].y;
    return y;

}
int find_the_breadth(vector<Points>temp)             // its for calculating the breadth, i am doing this becz to know whether i have to go for x split or y split
{
 sort(temp.begin(),temp.end(),sort_it_in_y);
 int n=temp.size();
 return abs(temp[0].y - temp[n-1].y);
}

int find_the_length(vector<Points>temp)              //its for calculating the length, i am doing this becz to know whether i have to go for x split or y split
{
    sort(temp.begin(),temp.end(),sort_it_in_x);
    int n=temp.size();
    return abs(temp[0].x - temp[n-1].x);
}

void insertion_function()                            // this is the heart of the program, its the insert function
{

    REP(i,total_number_points_inside_the_grid.size())              // inserting it one by one
    {
        Points elements=total_number_points_inside_the_grid[i];         // current element
        bool has_got_bucket=false;                                      // has allocated or not into the bucket, false means not allocated in any bucket
        int lx,rx,ly,ry,found_index;
        REP(j,Bucket_list_vector.size())                                 // i am trversing every bucket to know where current element may lie
        {
            if(has_got_bucket)break;                                      // if it got allocated then simply break

            if(check_if_this_points_lie_in_the_coordinates(elements,Bucket_list_vector[j].lower_left_x,Bucket_list_vector[j].lower_left_y,Bucket_list_vector[j].upper_right_x,Bucket_list_vector[j].upper_right_y))              // it returns true if the element lie in the given range
            {

                        // it means it lie in the given co-ordinates only
                        if(Bucket_list_vector[j].Points_inside_this_bucket.size()<bucket_size ) // simple thing, add it and marks that point added
                           {
                               Bucket_list_vector[j].Points_inside_this_bucket.push_back(elements);
                               has_got_bucket=true;
                           }
                        lx=Bucket_list_vector[j].lower_left_x;                    // for splitting purpose
                        ly=Bucket_list_vector[j].lower_left_y;                    // for splitting purpose
                        rx=Bucket_list_vector[j].upper_right_x;
                        ry=Bucket_list_vector[j].upper_right_y;
                        found_index=j;                                        // because this only needs to be further divide so i am keeping track of everything

            }
        }
        if(has_got_bucket==false)
        {
             // Now we have to know whether it is already divided or not, if yes then go with that if it divides, else divide it

             // find if we have the x_scale


             bool has_x_division=false,has_y_division=false;
             for(int k=lx+1;k<rx && !has_x_division;k++)                  // checking the splits in between in x direction
             {

                if(X_Scales.find(k)!=X_Scales.end()) // Now we have to divide along this line
                {
                    // we have to see along this line is it good to divide it
                    if(check_for_feasible_divide_along_x_direction(lx,ly,k,ry,rx,found_index,elements))       // it returns true means it got divide and store back in the global variable
                       {
                           has_x_division=true;           // marking that points that its allocated

                        }



                }

             }

             if(!has_x_division) // it means it does not fit in the x division point so we go for checking the y division line
             {

                for(int k=ly+1;k<ry &&!has_y_division;k++)
                {
                    if(Y_Scales.find(k)!=Y_Scales.end())
                    if(check_for_feasible_divide_along_y_direction(lx,ly,k,ry,rx,found_index,elements))         // same as above checking for prev y splits
                    {
                    has_y_division=true;

                    }
                }



             }
            bool has_divide_in_x=false,has_divide_in_y=false;
             if(!has_x_division && !has_y_division)   // now mean we have to draw a line
             {
                  vector<Points> first,second,temp=Bucket_list_vector[found_index].Points_inside_this_bucket;
                   temp.push_back(elements);
                   int len=find_the_length(temp);     // as discussed above
                   int bre=find_the_breadth(temp);

                if(len>=bre) // draw a vertical line now
                {
                   // now we have to divide everything



                    sort(temp.begin(),temp.end(),sort_it_in_x);
                    int x_divide=find_the_median_in_x(temp);

                    first.clear();
                    second.clear();
                    has_divide_in_x=true;
                    int left_side_lx=lx,left_side_ly=ly,left_side_rx=x_divide,left_side_ry=ry,right_side_lx=x_divide,right_side_ly=ly,right_side_rx=rx,right_side_ry=ry;
                    for(Points p:temp)
                    {
                        int curr_x=p.x,curr_y=p.y;
                        if(curr_x<left_side_rx && curr_x>=left_side_lx && curr_y>=left_side_ly &&curr_y<left_side_ry &&first.size()<bucket_size)
                            first.push_back(p);
                        else
                            second.push_back(p);

                    }
                    // upto this point i have divide the points in two bucket and now i have to check whether these division are to be implemented , this condition fails when the whose point lie in the same x axis line
                if(first.size()<=bucket_size && second.size()<=bucket_size && first.size()+second.size()==temp.size()){
                    Bucket_list_vector[found_index].lower_left_x=left_side_lx;
                    Bucket_list_vector[found_index].lower_left_y=left_side_ly;
                    Bucket_list_vector[found_index].upper_right_x=left_side_rx;
                    Bucket_list_vector[found_index].upper_right_y=left_side_ry;
                    Bucket_list_vector[found_index].Points_inside_this_bucket=first;
                    X_Scales.insert(x_divide);
                    Bucket b;
                    b.lower_left_x=right_side_lx;
                    b.lower_left_y=right_side_ly;
                    b.upper_right_x=right_side_rx;
                    b.upper_right_y=right_side_ry;
                    b.Points_inside_this_bucket=second;
                    Bucket_list_vector.push_back(b);


                }
                else
                    has_divide_in_x=false;
                }
               if(has_divide_in_x==false || bre>len)            // draw a horizontal line now , as its not been able to divide in x axis
                {
                    // now we have to divide everything


                    first.clear();
                    second.clear();
                    sort(temp.begin(),temp.end(),sort_it_in_y);
                    int y_divide=find_the_median_in_y(temp);

                    int lower_lx=lx,lower_ly=ly,lower_rx=rx,lower_ry=y_divide,upper_lx=lx,upper_ly=y_divide,upper_rx=rx,upper_ry=ry;
                    for(Points p:temp)
                    {
                        int curr_x=p.x,curr_y=p.y;
                        if(curr_x>=lower_lx &&curr_x<lower_rx &&curr_y>=lower_ly &&curr_y<lower_ry &&first.size()<bucket_size)
                            first.push_back(p);
                        else
                            second.push_back(p);
                    }
                    // upto this line, the points are added to the two buckets and now i have to check whether the points divided are valid or not
                    if(first.size()<=bucket_size && second.size()<=bucket_size){     // it means valid division
                    Bucket_list_vector[found_index].lower_left_x=lower_lx;
                    Bucket_list_vector[found_index].lower_left_y=lower_ly;
                    Bucket_list_vector[found_index].upper_right_x=lower_rx;
                    Bucket_list_vector[found_index].upper_right_y=lower_ry;
                    Bucket_list_vector[found_index].Points_inside_this_bucket=first;
                    has_divide_in_y=true;
                    Bucket b;
                    Y_Scales.insert(y_divide);
                    b.lower_left_x=upper_lx;
                    b.lower_left_y=upper_ly;
                    b.upper_right_x=upper_rx;
                    b.upper_right_y=upper_ry;
                    b.Points_inside_this_bucket=second;
                    Bucket_list_vector.push_back(b);
                    }
                    else
                        has_divide_in_y=false;

                }

            if(has_divide_in_x==false && has_divide_in_y==false)       // this is last case , first i have check length>=breadth then first try for x axis split and if not giving the valid result then go for y axis split, and when breadth>length then first divide it via y axis line and if it doent sound good means not a valid divion then go for x axis split
            {
               sort(temp.begin(),temp.end(),sort_it_in_x);
                    int x_divide=find_the_median_in_x(temp);

                    first.clear();
                    second.clear();
                    has_divide_in_x=true;
                    int left_side_lx=lx,left_side_ly=ly,left_side_rx=x_divide,left_side_ry=ry,right_side_lx=x_divide,right_side_ly=ly,right_side_rx=rx,right_side_ry=ry;
                    for(Points p:temp)
                    {
                        int curr_x=p.x,curr_y=p.y;
                        if(curr_x<left_side_rx && curr_x>=left_side_lx && curr_y>=left_side_ly &&curr_y<left_side_ry &&first.size()<bucket_size)
                            first.push_back(p);
                        else
                            second.push_back(p);

                    }
                    Bucket_list_vector[found_index].lower_left_x=left_side_lx;
                    Bucket_list_vector[found_index].lower_left_y=left_side_ly;
                    Bucket_list_vector[found_index].upper_right_x=left_side_rx;
                    Bucket_list_vector[found_index].upper_right_y=left_side_ry;
                    Bucket_list_vector[found_index].Points_inside_this_bucket=first;
                    X_Scales.insert(x_divide);
                    Bucket b;
                    b.lower_left_x=right_side_lx;
                    b.lower_left_y=right_side_ly;
                    b.upper_right_x=right_side_rx;
                    b.upper_right_y=right_side_ry;
                    b.Points_inside_this_bucket=second;
                    Bucket_list_vector.push_back(b);


            }           // upto this line the points have been added by the division of bucket

             }



        }


    }


}
string convert_it_to_string(Points p)     // it return as a string of x and y co-ordinate
{
    string s=to_string(p.x)+ " "+ to_string(p.y);
    return s;
}
 int file_counter=0;   // global counter for counting the bucket number
void write_it_back()        // it writes everything in text file , that is the points in each bucket and name them accordingly
{
    int n=Bucket_list_vector.size();

    REP(i,n)
    {
        if(Bucket_list_vector[i].Points_inside_this_bucket.size()>0)
        {
            string name="bucket "+to_string(file_counter)+" .txt";
            fstream file;
            file.open(name,ios::out);
            for(Points p:Bucket_list_vector[i].Points_inside_this_bucket)
            {
                file<<convert_it_to_string(p)<<endl;
            }
            file.close();
            file_counter++;
        }

    }

}
void call_function_for_1_bucket_size()       // i have created another function for handling the case when bucket size is 1
{
    for(Points p:total_number_points_inside_the_grid)
    {
        fstream file;
        string name="Bucket "+to_string(file_counter)+" .txt" ;

        file.open(name,ios::out);
        file<<convert_it_to_string(p)<<endl;
        file.close();
        X_Scales.insert(p.x);
        Y_Scales.insert(p.y);
        Bucket b;
        b.lower_left_x=b.upper_right_x=p.x;
        b.upper_right_y=b.lower_left_y=p.y;
        b.Points_inside_this_bucket.push_back(p);
        Bucket_list_vector.push_back(b);
        file_counter++;
    }


}
void print_x_y_scale()                                       // for print the split of x and y axis
{
    cout<<"The X_scales splits are \n";
    for(auto x= X_Scales.begin();x!=X_Scales.end();x++)
    cout<<*x<<" ";
    cout<<"\nThe Y_scales splits are \n";
    for(auto x= Y_Scales.begin();x!=Y_Scales.end();x++)
    cout<<*x<<" ";
    cout<<endl;
}
pair<int,int> fetch_the_x_y_from_string(string line)
{
    vector <string> tokens;


    stringstream check1(line);

    string intermediate;


    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }
    pair<int,int> res;
    res.first=stoi(tokens[0]);
    res.second=stoi(tokens[1]);
    return res;

}

void delete_that_point_from_that_list(string s)              // function to delete already allocated points from the total number of points
{
    Points p;
    pair<int,int> rr=fetch_the_x_y_from_string(s);
    p.x=rr.first;
    p.y=rr.second;

    REP(i,total_number_points_inside_the_grid.size())
    {
        if(total_number_points_inside_the_grid[i].x==p.x && total_number_points_inside_the_grid[i].y==p.y)
        {
            total_number_points_inside_the_grid.erase(total_number_points_inside_the_grid.begin()+i);

            return;

        }

    }



}

void check_for_overflow_and_remove_that(){                  // this is for handling the repeated points at the same location for overflow condition
    int counter=0;
for(auto itr=my_map.begin();itr!=my_map.end();itr++)
{
    if(itr->second > bucket_size)
    {
        string name="overflow bucket "+to_string(counter)+" .txt";
        fstream file;
        cout<<itr->first<<endl;
        string s=itr->first;
        file.open(name,ios::out);
        while(itr->second > bucket_size){
            file<<s<<endl;itr->second--;
            delete_that_point_from_that_list(itr->first);}
        counter++;
        file.close();


    }

}

}

void print_the_bucket()
{
    for(Bucket b:Bucket_list_vector)
    {
        cout<<"\nThe lower left x,y is "<<b.lower_left_x<<" "<<b.lower_left_y<<endl;
        cout<<"The upper right x,y is "<<b.upper_right_x<<" "<<b.upper_right_y<<endl;
        cout<<"The Points inside this buckets are "<<endl;
        for(Points p:b.Points_inside_this_bucket)
        {
            cout<<p.x<<" "<<p.y<<endl;
        }


    }

}

int main(){


srand(time(0));

create_dataset();
                                        // to create random dataset, uncomment it to create your own dataset
X_Scales.clear();
Y_Scales.clear();

int lower_left_x,lower_left_y,upper_right_x,upper_right_y;
cout<<"Bucket size is ?\n";
cin>>bucket_size;
cout<<"Enter the lower left and the upper right coordinates in (x,y) type\n";

cin>>lower_left_x>>lower_left_y>>upper_right_x>>upper_right_y;

calculate_the_total_points_which_are_inside_the_grid(lower_left_x,upper_right_x, lower_left_y,upper_right_y);


check_for_overflow_and_remove_that();
Bucket b;
b.lower_left_x=lower_left_x;
b.lower_left_y=lower_left_y;
b.upper_right_x=upper_right_x;
b.upper_right_y=upper_right_y;
Bucket_list_vector.push_back(b);

if(bucket_size==1)          // in this case only 1 point can lie inside the grid , so i am considering the point grid and everything got divided
call_function_for_1_bucket_size();
else
{
    insertion_function();
    write_it_back();
}

print_x_y_scale();
print_the_bucket();
return 0;


}

/*




Step by step process of my program

1. first i fetch the total number of points inside that grid,
2. i check for overflow condition , that is if there are multiple points > bucket size at the same location , first resolve that

3. now i am calling a insert function, it inserts one by one element into the bucket, and if anytime the size increase > bucket size then split, while splitting you have see first that whether there
is some previous splits have been done or not if done then divide that bucket according to that line , check for this for both lines(horizontal and vertical)

4.At this step I am writing everything on the text file

*/
