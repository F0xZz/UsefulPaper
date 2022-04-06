    # int missingNumber(int[] nums) {
    #     int n = nums.length;
    #     int res = 0;
    #     // 先和新补的索引异或一下
    #     res ^= n;
    #     // 和其他的元素、索引做异或
    #     for (int i = 0; i < n; i++)
    #         res ^= i ^ nums[i];
    #     return res;
    # }
def missing(listNum:list):
    n = len(listNum)
    res = 0
    res = res^ n 
    for i in range(n):
        print()
        res = res^(i^listNum[i])
    return res
a = [0,1,2,4,5]
print(missing(a))