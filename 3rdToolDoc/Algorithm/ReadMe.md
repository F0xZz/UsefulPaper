#prime number find out count

question 1:

give you a number list [2,n] solve the question about count how much prime number in this list 

optim 1. loop in the sqrt(n)

```c++
for (i = 2;i*i<n;i+=1){
// *** make sure which nuber is prime
    if(isprime(i)){
        count++
    }
}
```

the function about is prime number that you can know 

optim 2. if you have a **prime numbe**  <font color=#FF0000 >P</font> this number <font color=#FF0000 >P*(P+n)</font>

```c++
boolean[] isPrim = new boolean[n];
Arrays.fill(isPrim,true);
for (i = 2;i*i<n;i+=1){
        if (isPrim[i]) 
            for (int j = i * i; j < n; j += i) 
                isPrim[j] = false;
}
//else count the true
```

question 2:

LIS (longest incresasing subsequence )最长子序列可不连续，

serial 就必须连续了
