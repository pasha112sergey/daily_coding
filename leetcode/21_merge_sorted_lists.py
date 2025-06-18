# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, val=0, next=None):
#         self.val = val
#         self.next = next
class Solution(object):
    def mergeTwoLists(self, list1, list2):
        """
        :type list1: Optional[ListNode]
        :type list2: Optional[ListNode]
        :rtype: Optional[ListNode]
        """
        """
        1. Start at min value
        2. Choose its new next between its next value or the other list
        3. Set its next to the one chosen by step 2 and move curr to that node
        """
        ans = list1
        if list1 == None:
            return list2
        if list2 == None:
            return list1
        other = None
        if list1.val > list2.val:
            ans = list2
            other = list1
        else:
            ans = list1
            other=list2
        curr = ans
        while curr.next:
            if curr.next.val < other.val:
                curr = curr.next
            else:
                temp = curr.next
                curr.next = other
                curr = other
                other = temp
        
        curr.next = other
        return ans

