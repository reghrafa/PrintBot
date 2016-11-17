using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace PrintBot.Cloud.CompilerService.Controllers
{
    [Route("api/[controller]")]
    public class CompileController : Controller
    {
        // POST api/compile
        [HttpPost]
        public void Post([FromBody]SourceCodeDto dto)
        {
            if (!ModelState.IsValid)
            {
                return;
            }

        }
    }
}
