-- Description of experiments for which we want to generate XML files
experiments = {
   -- 20 robots, small arena, return to nest
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 123, "footbot_foraging_controller") 
   },
   -- 20 robots, small arena, DON't return to nest
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 123, "footbot_foraging_controller_nr") 
   },
   --- rinse, repeat, etc.
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 124, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 124, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 125, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 125, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 126, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 126, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 127, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 127, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 128, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 128, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 129, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 129, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 130, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 130, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 131, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 131, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 132, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 132, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 133, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 133, "footbot_foraging_controller_nr") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 134, "footbot_foraging_controller") 
   },
   { file_prefix = "./xml/",
     substitution_table = build_experiment_table(20, 4, 36000, 134, "footbot_foraging_controller_nr") 
   }
}
