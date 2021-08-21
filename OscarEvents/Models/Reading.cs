using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Azure.Cosmos.Table;

namespace OscarEvents.Models
{
    class Reading : TableEntity
    {
        public Reading()
        {

        }
        public Reading(string customer, DateTime dt)
        {
            PartitionKey = customer;
            RowKey = dt.ToString("s");

        }
        public string values { get; set; }
        
    }
    public class Triple
    {
        public double x { get; set; }
        public double y { get; set; }
        public double z { get; set; }

        public Triple(string s)
        {
            string[] val = s.Split(new char[] { ',' });
            this.x = Convert.ToDouble(val[0]);
            this.y = Convert.ToDouble(val[1]);
            this.z = Convert.ToDouble(val[2]);
        }

        internal static List<Triple> FromSensor(string values)
        {
            List<Triple> triples = new List<Triple>();
            string[] vals = values.Split(new char[] { ';' });
            foreach(string val in vals)
            {
                triples.Add(new Triple(val));
            }
            return triples;
        }
        public static bool isSeizure(IEnumerable<Triple> triples)
        {
            bool test1=false, test2 = false;
            //Check if there is a high amplitude
            var xVals = triples.Select(i => i.x);
            double max = xVals.Max();
            double min = xVals.Min();
            if (max - min > 3)
                test1 = true;
            var yVals = triples.Select(i => i.y);
            max = yVals.Max();
            min = yVals.Min();
            if (max - min > 3)
                test1 = true;
            var zVals = triples.Select(i => i.z);
            max = zVals.Max();
            min = zVals.Min();
            if (max - min > 3)
                test1 = true;


            //check if there is a consistent movement
            Triple prev = null;
            int count = 0;
            foreach(Triple t in triples)
            {
                if (prev == null)
                {
                    prev = t;
                    continue;
                }
                if (Math.Abs(prev.x - t.x) < 0.5 && Math.Abs(prev.y - t.y) < 0.5 && Math.Abs(prev.z - t.z) < 0.5)
                    count++;
            }
            if (count < 4)
                test2 = true;
            //if both true return true
            if (test1 && test2)
                return true;
            else return false; 
        }
    }
}
