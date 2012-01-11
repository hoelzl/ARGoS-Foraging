-- Description of experiments for which we want to generate XML files
experiments = {
   -- 20 robots, small arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 3600, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 3600, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 3600, 125, "libfootbot_foraging.so") 
   },

   -- 20 robots, big arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 3600, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 3600, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 8, 3600, 125, "libfootbot_foraging.so") 
   },

   -- 40 robots, small arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 3600, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 3600, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 4, 3600, 125, "libfootbot_foraging.so") 
   },

   -- 40 robots, big arena
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 3600, 123, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 3600, 124, "libfootbot_foraging.so") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(40, 8, 3600, 125, "libfootbot_foraging.so") 
   }
}
