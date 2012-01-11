#!/usr/bin/env lua

-- This kinda sucks.  Is there no better way without installing a
-- posix library?
function file_exists (filename)
   local file = io.open(filename)
   if file then
      io.close(file)
      return true
   else
      return false
   end
end

if file_exists("xml-template.lua") then
   dofile("xml-template.lua")
elseif file_exists("scripts/xml-template.lua") then
   dofile("scripts/xml-template.lua")
else
   error("Cannot find the XML template")
end

-- This table contains all substitutions present in the xml template.
default_substitutions = {
   ["%experiment-number%"]       = 0,
   ["%arena-size%"]              = "4",
   ["%experiment-length%"]       = "36000",
   ["%random-seed%"]             = "123",
   ["%foraging-controller-tag%"] = "footbot_foraging_controller",
   ["%foraging-controller%"]     = "libfootbot_foraging.so",
   ["%user-function%"]           = "libforaging_loop_functions.so",
   ["%trace-output%"]            = "results/1-foraging.csv",
   ["%collision-output%"]        = "results/1-collisions.csv",
   ["%id-for-collision-output%"] = "1",
   ["%summary-output%"]          = "results/1-summary.csv",
   ["%wall-north-position%"]     = "0,2,0.25",
   ["%wall-north-size%"]         = "4,0.1,0.5", 
   ["%wall-south-position%"]     = "0,-2,0.25",
   ["%wall-south-size%"]         = "4,0.1,0.5",
   ["%wall-east-position%"]      = "2,0,0.25",
   ["%wall-east-size%"]          = "0.1,4,0.5",
   ["%wall-west-position%"]      = "-2,0,0.25",
   ["%wall-west-size%"]          = "0.1,4,0.5",
   ["%light-1-position%"]        = "-2,-1.5,1.0",
   ["%light-2-position%"]        = "-2,-0.5,1.0",
   ["%light-3-position%"]        = "-2,0.5,1.0",
   ["%light-4-position%"]        = "-2,1.5,1.0",
   ["%position-min%"]            = "-2,-2,0",
   ["%position-max%"]            = "-1,2,0",
   ["%orientation-min%"]         = "0,0,0",
   ["%orientation-max%"]         = "360,0,0",
   ["%number-of-robots%"]        = "20",
   ["%user-function%"]           = "libforaging_loop_functions.so"
}

function table.print(tab)
   for k,v in pairs(tab) do
      print(k,v)
   end
end

function table.merge_destructively (tab, ...)
   for _,t in ipairs{...} do
      for k,v in pairs(t) do
	 tab[k] = v
      end
   end
   return tab
end

function table.merge (...)
   return table.merge_destructively({}, ...)
end

function table.equal (t1, t2)
   for k,v in pairs(t1) do
      if t2[k] ~= v then
	 print("table 1: ", k, v, "table 2:", k, t2[k])
	 return false
      end
   end
   for k,v in pairs(t2) do
      if t1[k] ~= v then
	 print("table 1: ", k, v, "table 2:", k, t2[k])
	 return false
      end
   end
   return true
end

function light_pos (index, size)
   local half_size = size / 2
   local quarter_size = size / 4
   local eigth_size = size / 8
   return tostring(-half_size + eigth_size + (index - 1) * quarter_size)
end

function square_arena (size)
   local half_size = size / 2
   local s, hs = tostring(size), tostring(half_size)
   local pos = { light_pos(1, size), light_pos(2, size),
		 light_pos(3, size), light_pos(4, size) }
   tab = { ["%arena-size%"]              = s,
	   ["%wall-north-position%"]     = "0," .. hs .. ",0.25",
	   ["%wall-north-size%"]         = s .. ",0.1,0.5", 
	   ["%wall-south-position%"]     = "0,-" .. hs .. ",0.25",
	   ["%wall-south-size%"]         = s .. ",0.1,0.5",
	   ["%wall-east-position%"]      = hs .. ",0,0.25",
	   ["%wall-east-size%"]          = "0.1," .. s .. ",0.5",
	   ["%wall-west-position%"]      = "-" .. hs .. ",0,0.25",
	   ["%wall-west-size%"]          = "0.1," .. s .. ",0.5",
	   ["%light-1-position%"]        = "-" .. hs .. "," .. pos[1] .. ",1.0",
	   ["%light-2-position%"]        = "-" .. hs .. "," .. pos[2] .. ",1.0",
	   ["%light-3-position%"]        = "-" .. hs .. "," .. pos[3] .. ",1.0",
	   ["%light-4-position%"]        = "-" .. hs .. "," .. pos[4] .. ",1.0",
	   ["%position-min%"]            = "-" .. hs .. ",-" .. hs .. ",0",
	   ["%position-max%"]            = "-" .. tostring(hs-1) .. "," .. hs .. ",0"
   }
   return tab
end

-- print(table.equal(default_substitutions,
-- 		  table.merge(default_substitutions, square_arena(4))))
-- --> true

function files_for_experiment (number)
   n = string.format("%03d", number)
   tab = { ["%experiment-number%"]       = number,
	   ["%trace-output%"]            = "results/" .. n .. "-foraging.csv",
	   ["%collision-output%"]        = "results/" .. n .. "-collisions.csv",
	   ["%summary-output%"]          = "results/" .. n .. "-summary.csv"
   }
   return tab
end

function build_experiment_table (number_of_robots, arena_size, experiment_length,
				 random_seed, controller_tag)
   return table.merge(default_substitutions,
		      square_arena(arena_size),
		      { ["%experiment-length%"]       = experiment_length,
			["%random-seed%"]             = random_seed,
			["%foraging-controller-tag%"] = controller_tag,
			["%number-of-robots%"]        = number_of_robots
		      })
end

-- table.print(build_experiment_table(20, 4, 3600, 123, "footbot_foraging_controller"))

function build_xml_file (substitution_table)
   return (string.gsub(xml_template, "%%[^%%]+%%", substitution_table))
end

-- print(build_xml_file(build_experiment_table(20, 4, 3600, 123, "footbot_foraging_controller")))

function write_xml_file_for_experiment (file_prefix, substitution_table)
   local file_name = string.format("%s/%03d-xp.xml", 
				   file_prefix,
				   substitution_table["%experiment-number%"])
   local file = assert(io.open(file_name, "w"))
   file:write(build_xml_file(substitution_table))
   file:close()
end

-- write_xml_file_for_experiment("/tmp/", build_experiment_table(1, 20, 4, 3600, 123, "footbot_foraging_controller"))

function write_xml_file_for (exps)
   -- Do some sanity checks first and assign numbers
   print("Doing sanity checks.")
   for i,exp in ipairs(exps) do
      assert(exp.file_prefix,
	     "Experiment must contain a file prefix.")
      assert(exp.substitution_table,
	     "Experiment must contain a substitution table.")
      assert(type(exp.substitution_table) == "table",
	    "The substitution table must be a table.")
      table.merge_destructively(exp.substitution_table,
				files_for_experiment(i))
   end
   -- OK, since we got here all entries are at least somewhat sane...
   print("Sanity checks passed, writing XML files.")
   for _,exp in ipairs(exps) do
      write_xml_file_for_experiment(exp.file_prefix, exp.substitution_table)
   end
   print("All files written to disk.")
end

experiments = nil
if arg then
   if arg[1] and not arg[2] then
      if file_exists(arg[1])  then
	 dofile(arg[1])
	 if experiments then
	    write_xml_file_for(experiments)
	 else
	    print("File " .. arg[1] .. " contains no experiments.")
	 end
      else
	 print("File " .. arg[1] .. " does not exist.")
      end
   else
      print("Usage: " .. arg[0] .. " <experiment-description-file>")
   end
else
   print("No arguments passed to the program.")
end
