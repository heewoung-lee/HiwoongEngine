namespace ConsoleApp1
{

    class Player
    {
        public int Hp;
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            Player original = new Player { Hp = 50 };
            Change(original);

            object b = 10;
            int a = Convert.ToInt32(b);   // 정상
            long c = Convert.ToInt64(b); // InvalidCastException

        }


        static void Change(Player player)
        {
            player.Hp = 0;
            player = new Player { Hp = 100 };
        }
    }
}







