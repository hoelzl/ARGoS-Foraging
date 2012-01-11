-- Description of experiments for which we want to generate XML files
experiments = {
   -- 20 robots, small arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 125, "libfootbot_foraging.so") 
   },

   -- 20 robots, big arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 36000, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 36000, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 36000, 125, "libfootbot_foraging.so") 
   },

   -- 40 robots, small arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 36000, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 36000, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 36000, 125, "libfootbot_foraging.so") 
   },

   -- 40 robots, big arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 36000, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 36000, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 36000, 125, "libfootbot_foraging.so") 
   }
}
