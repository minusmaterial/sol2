#!/bin/bash

grep \#include main.c | awk '{print $2}' 
