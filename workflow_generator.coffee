#!/usr/bin/env coffee
fs = require 'fs'

doc = """
Usage:
  workflow_generator.coffee [options] <tierCount> <outfile> [--interfaceSize=<n>] [--a1Size=<n>] [--anSize=<n>] [--leafSize=<n>]
  
  --interfaceSize=<n>   Size of iter interface [default: 5]
  --leafSize=<n>        Size of intermediate tier matrices [default: 17]
  --a1Size=<n>          Size of intermediate first tier matrix [default: 21]
  --anSize=<n>          Size of intermediate last tier matrix [default: 21]
"""
{docopt} = require 'docopt'

args = docopt(doc, version: '1.0')

interfaceSize = parseInt(args['--interfaceSize'])
leafSize      = parseInt(args['--leafSize'])
a1Size        = parseInt(args['--a1Size'])
anSize        = parseInt(args['--anSize'])
tierCount     = parseInt(args['<tierCount>'])

generate_tree = -> 
  root = {type: "root", id: "root"}
  id = 0
  next_id = -> (id += 1)
  gen_leaf = (idx) -> 
    if idx == 0
      {type: "leaf_A1", id: next_id(), tier: idx}
    else if idx == tierCount - 1
      {type: "leaf_AN", id: next_id(), tier: idx}
    else
      {type: "leaf", id: next_id(), tier: idx}
    
  gen_level = (low, high, parent) ->
    if (high - low) > 2
      parent.left = {type: "eliminate", id: next_id()}
      parent.right = {type: "eliminate", id: next_id()}
      
      gen_level low, low + Math.floor((high-low) / 2), parent.left
      gen_level low + Math.floor((high-low) / 2) + 1, high, parent.right
    else if (high - low) == 2 
      parent.left = (gen_leaf low)
      parent.right = {type: "eliminate", id: next_id()}
      parent.right.left = (gen_leaf low + 1)
      parent.right.right = (gen_leaf low + 2)
    else if (high - low) == 1
      parent.left = (gen_leaf low)
      parent.right = (gen_leaf low+1)
  
  gen_level(0, tierCount-1, root)
  root
  
generate_workflow = (tree) ->
  workflow = 
    functions: [
      {
        "name": "command",
        "module": "functions"
      }
    ]
  tasks = [
        {
          name: "solve",
          function: "command",
          type: "dataflow",
          firingLimit: 1,
          config: {
            executor: {
              executable: "solve",
              args: "#{interfaceSize} #{tree.left.id}_export #{tree.right.id}_export #{tree.left.id}_rhs #{tree.right.id}_rhs"
            },
          }
          ins: ["#{tree.left.id}_export", "#{tree.right.id}_export"],
          outs: ["#{tree.left.id}_rhs", "#{tree.right.id}_rhs"]
        }
  ]
    
  generate_bs = (subtree) ->
    
    add_leaf = (tier_index, subtree_id, size, type) ->
      tasks.push({
        name: "down_A",
        function: "command",
        type: "dataflow",
        firingLimit: 1,
        config: {
          executor: {
            executable: "down_A",
            args: "#{type} #{size} #{interfaceSize} tier_#{tier_index}_state #{subtree_id}_rhs tier_#{tier_index}_solution"
          }
        },
        ins: ["tier_#{tier_index}_state", "#{subtree_id}_rhs"],
        outs: ["tier_#{tier_index}_solution"]
      })
    add_postprocess = (tier_index) ->
      tasks.push({
        name: "postprocess",
        function: "command",
        type: "dataflow",
        firingLimit: 1,
        config: {
          executor: {
            executable: "postprocessor",
            args: "#{tierCount} #{tier_index} tier_#{tier_index}_solution tier_#{tier_index}_results"
          }
        },
        ins: ["tier_#{tier_index}_solution"],
        outs: ["tier_#{tier_index}_results"]
      })
    
    switch subtree.type
      when "eliminate"
        generate_bs(subtree.left)
        generate_bs(subtree.right)
        tasks.push({
          name: "down",
          function: "command",
          type: "dataflow",
          firingLimit: 1,
          config: {
            executor: {
              executable: "down",
              args: "#{interfaceSize} #{subtree.id}_state #{subtree.id}_rhs #{subtree.left.id}_rhs #{subtree.right.id}_rhs"
            },
          }
          ins: ["#{subtree.id}_state", "#{subtree.id}_rhs"],
          outs: ["#{subtree.left.id}_rhs", "#{subtree.right.id}_rhs"]
        })
      when "leaf"
        add_leaf subtree.tier, subtree.id, leafSize, 'A'
        add_postprocess subtree.tier
      when "leaf_A1"
        add_leaf subtree.tier, subtree.id, a1Size, '1'
        add_postprocess subtree.tier
      when "leaf_AN"
        add_leaf subtree.tier, subtree.id, anSize, 'N'
        add_postprocess subtree.tier
        
  generate_eliminate = (subtree) ->
    
    add_generate_tier = (index) -> 
      tasks.push({
        name: "generate_tier",
        function: "command",
        type: "dataflow",
        firingLimit: 1,
        config: {
          executor: {
            executable: "generate_tier",
            args: "#{tierCount} #{index} tier_#{index}"
          }
        },
        outs: ["tier_#{index}"],
        ins: []
      })
    add_leaf = (tier_index, subtree_id, size, type = '') ->
      tasks.push({
        name: "production_A#{type}",
        function: "command",
        type: "dataflow",
        firingLimit: 1,
        config: {
          executor: {
            executable: "production_A#{type}",
            args: "#{size} #{interfaceSize} tier_#{tier_index} tier_#{tier_index}_state #{subtree_id}_export"
          }
        },
        outs: ["tier_#{tier_index}_state", "#{subtree_id}_export"],
        ins: ["tier_#{tier_index}"]
      })
    
    switch subtree.type
      when "eliminate"
        tasks.push({
          name: "merege",
          function: "command",
          type: "dataflow",
          firingLimit: 1,
          config: {
            executor: {
              executable: "merge",
              args: "#{interfaceSize} #{subtree.left.id}_export #{subtree.right.id}_export #{subtree.id}_state #{subtree.id}_export"
            },
          }
          ins: ["#{subtree.left.id}_export", "#{subtree.right.id}_export"],
          outs: ["#{subtree.id}_state", "#{subtree.id}_export"]
        })
        generate_eliminate(subtree.left)
        generate_eliminate(subtree.right)
      when "leaf"
        add_generate_tier subtree.tier
        add_leaf subtree.tier, subtree.id, leafSize
      when "leaf_A1"
        add_generate_tier subtree.tier
        add_leaf subtree.tier, subtree.id, a1Size, '1'
      when "leaf_AN"
        add_generate_tier subtree.tier
        add_leaf subtree.tier, subtree.id, anSize, 'N'
        
  generate_eliminate(tree.left)
  generate_eliminate(tree.right)
  generate_bs(tree.left)
  generate_bs(tree.right)
  
  workflow.processes = tasks
  workflow.ins = ([0...tierCount].map (i) -> "tier_#{i}" )
  workflow.outs = ([0...tierCount].map (i) -> "tier_#{i}_solution" )
  workflow.signals = [].concat.apply([], tasks.map (t) -> t.outs).map((t) -> {name: t})
  
  workflow

tree = generate_tree()
workflow =generate_workflow tree

# console.log JSON.stringify(tasks, null, 4)

fs.writeFile args['<outfile>'], (JSON.stringify workflow, null, 4), (err) ->
    if err
        console.log err
        process.exit 1
    else
        console.log "File #{args['<outfile>']} written with workflow of #{workflow.processes.length} tasks!"
