import java.util.*;

public class Main {
    public static int Q;
    public static Map<Integer, Integer> isConstructed;
    public static List<Integer> history = new ArrayList<>();
    public static void main(String[] args) {
        
        Scanner sc = new Scanner(System.in);
        Q = sc.nextInt();
        
        isConstructed = new HashMap<>();

        for(int i=0;i<Q;i++)
        {
            int cmd = sc.nextInt();

            if(cmd == 100)
            {
                int n = sc.nextInt();
                for(int j=0;j<n;j++)
                {
                    int home = sc.nextInt();
                    isConstructed.put(history.size(), 1);
                    history.add(home);
                }
            }else if(cmd == 200)
            {
                int home = sc.nextInt();
                isConstructed.put(history.size(), 1);
                history.add(home);
            }else if(cmd == 300)
            {
                int home = sc.nextInt();
                isConstructed.put(home,0);
            }else if(cmd == 400)
            {
                int r;
                r = sc.nextInt();

                List<Integer> temp = new ArrayList<>();
                for(int j=0;j<history.size();j++)
                {
                    int home = history.get(j);
                    if(isConstructed.get(j) == 1){
                        temp.add(home);
                    }
                }

                Collections.sort(temp);
                
                List<Integer> arr = new ArrayList<>(Collections.nCopies(temp.size(), 0));

                //System.out.println(temp.size() + " " + arr.size());

                for(int j=0;j<arr.size();j++) arr.set(j, 0);
                if(arr.size() == 0) {
                    System.out.println("0");
                    continue;
                }
                arr.set(0,1);

                history = temp;

                PriorityQueue<int[]> pq = new PriorityQueue<>(
                    (a, b) ->{
                        return -Integer.compare(a[0], b[0]);
                    }
                );

                for(int j=0;j<temp.size() - 1;j++)
                {
                    int dist = temp.get(j+1) - temp.get(j);
                    pq.add(new int[]{dist, j+1});
                }

                r--;

                while(!pq.isEmpty())
                {
                    if(r == 0) break;
                    int[] pos = pq.poll();

                    arr.set(pos[1], 1);
                    r--;
                }

                int cost = 0;
                int last = 0;

                for(int j=0;j<arr.size();j++)
                {
                    if(arr.get(j) != 1)
                    {
                        cost += temp.get(j) - last;
                    }

                    last = temp.get(j);
                }

                System.out.println(cost);
            }
        }
    }
}