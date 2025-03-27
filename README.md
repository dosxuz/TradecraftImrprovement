# Tradecraft Improvement

## Part 1: Creating a PE file with no imports

In this blog I discuss about how we can create a PE file without any imports.

## Part 2: Module Stomping

In this chapter I do a little introduction to module stomping

## Part 3: AMSI Bypass series - Reverse Engineering AMSI

In this chapter I dive into AMSI and reverse engineer a few components of AMSI

## Part 4: Hunting for AMSI bypasses

Using the knowledge in the previous blog I try to find out various types of AMSI bypasses in `amsi.dll`

## Part 5: Hooking into AMSI functions

In this blog I discuss how one can hook into various AMSI functions using Detours

## Part 6: Patching amsiContext and attacking AmsiInitialize

First I discuss how one can change the value of `amsiContext` structure. Then I dicuss how one can manipulate `System.Management.Automation.dll` to change the value of `amsiInitFailed` field.
