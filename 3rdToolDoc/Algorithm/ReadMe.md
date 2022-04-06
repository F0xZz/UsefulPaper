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



ListNode 类型链表反转



查阅资料

关于反转问题

1. 首先得保存当前节点的下一节点
2. 将上述节点指向上一个节点，实现反转
3. 将当前节点设置为pre=head
4. 将保存下一个节点为头结点

```
ListNode* rever(ListNode* head){
ListNode* cur=head;
ListNode* pre;
while(head!=nullptr){
cur = head->next;
head->next = pre;
pre = head ;
head = next ;
}
return pre;
}
```







```
ListNode reverse(ListNode a) {
    ListNode pre, cur, nxt;
    pre = null; cur = a; nxt = a;
    while (cur != null) {
        nxt = cur.next;
        // 逐个结点反转
        cur.next = pre;
        // 更新指针位置
        pre = cur;
        cur = nxt;
    }
    // 返回反转后的头结点
    return pre;
}
```

[a,d)

a-> b->c->d

```
ListNode reverse(ListNode a,ListNode d) {
    ListNode pre, cur, nxt;
    pre = null; cur = a; nxt = a;
    while (cur != d ) {
        nxt = cur.next;
        // 逐个结点反转
        cur.next = pre;
        // 更新指针位置
        pre = cur;
        cur = nxt;
    }
    // 返回反转后的头结点
    return pre;
}
```

链表删除倒数某个节点或者某个节点

```
非常简单的获取长度
    int getLength(ListNode* head) {
        int length = 0;
        while (head) {
            ++length;
            head = head->next;
        }
        return length;
    }
    
ListNode* dummy = new ListNode(0, head);
    int length = getLength(head);
    ListNode* cur = dummy;
    for (int i = 0; i < (length+1) - n-1; ++i) {
        cur = cur->next;
    }
    cur->next = cur->next->next;
    ListNode* ans = dummy->next;
    delete dummy;
    return ans;
```

关于(length+1)-n-1 因为length+1是dummy长度是length+1但是我需要遍历到倒数第N+1个位置所以又减去1

cur->next = cur->next->next;好理解 跳接

ListNode* ans = dummy->next;

好理解

delete

return



二叉树

前 中 后

前left-root-right 

中root-left-right

后right-root-left



```
listnode * pre;cur;nxt;
pre=nullptr;
while(head){
nxt =cur->next;
cur->next = pre;
pre = cur;
cur = nxt;
}
return pre;
```

1~n 的count 1

input n

```
 int digit = 1, res = 0;//先初始化，因为给一个数字，cur指针先指向最后一位，也就是个位，其十位，百位，，，都是high，是没有low位的，在后面的循环中，cur往前移动一位，则对应的high少一位，low就多一位
        int high = n / 10, cur = n % 10, low = 0;//这个也是初始化
        while(high != 0 || cur != 0) {
            if(cur == 0) res += high * digit;
            else if(cur == 1) res += high * digit + low + 1;
            else res += (high + 1) * digit;
            low += cur * digit;//计算完一轮的high，low，cur等指针会移动，下面是移动后的指针赋值
            cur = high % 10;
            high /= 10;
            digit *= 10;
        }
        return res;


```

