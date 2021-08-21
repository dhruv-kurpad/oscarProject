using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using Azure.Data.Tables;
using Microsoft.WindowsAzure.Storage;
using Microsoft.Azure.Cosmos.Table;
using OscarEvents.Models;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;

namespace OscarEvents
{
    public static class Function1
    {
        [FunctionName("ProcessEvents")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Function, "post", Route = null)] HttpRequest req,
            ILogger log)
        {
            log.LogInformation("C# HTTP trigger function processed a request.");
            string storageConnectionString = Environment.GetEnvironmentVariable("StorageConnectionString");

            Microsoft.Azure.Cosmos.Table.CloudStorageAccount storageAccount;
            try
            {
                storageAccount = Microsoft.Azure.Cosmos.Table.CloudStorageAccount.Parse(storageConnectionString);
            }
            catch (FormatException)
            {
                Console.WriteLine("Invalid storage account information provided. Please confirm the AccountName and AccountKey are valid in the app.config file - then restart the application.");
                throw;
            }
            catch (ArgumentException)
            {
                Console.WriteLine("Invalid storage account information provided. Please confirm the AccountName and AccountKey are valid in the app.config file - then restart the sample.");
                Console.ReadLine();
                throw;
            }
            CloudTableClient tableClient = storageAccount.CreateCloudTableClient(new TableClientConfiguration());
            CloudTable table = tableClient.GetTableReference("Readings");
            await table.CreateIfNotExistsAsync();
            Reading entity = new Reading("oscar", DateTime.Now);
            entity.values = await req.ReadAsStringAsync();
            TableOperation insertOrMergeOperation = TableOperation.InsertOrMerge(entity);
            List<Triple> readings = Triple.FromSensor(entity.values);
            if (Triple.isSeizure(readings))
            {
                var payload = new
                {
                    message = "Looks like Oscar is having a seizure",
                    user = "Dhruv"
                };

                var json = JsonConvert.SerializeObject(payload);
                var data = new StringContent(json, Encoding.UTF8, "application/json");
                var url = "your logic app URL";
                using (var client = new HttpClient())
                {
                    var response = await client.PostAsync(url, data);
                    string res = response.Content.ReadAsStringAsync().Result;

                }


            }


            // Execute the operation.
            TableResult result = await table.ExecuteAsync(insertOrMergeOperation);
            Reading insertedCustomer = result.Result as Reading;





            return new OkResult();
        }
    }
}
